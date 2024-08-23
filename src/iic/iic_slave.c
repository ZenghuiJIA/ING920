#include "iic_slave.h"
#include "peripheral_gpio.h"
#include "peripheral_sysctrl.h"
#include "misc.h"

#if 1
#include "SEGGER_RTT.h"
#include "debug_print.h"
#define DEBUG_LOG(...) SEGGER_RTT_printf(0, ##__VA_ARGS__)

#define DEBUG_UART_log debug_uart
#else
#define DEBUG_LOG(...)
#define DEBUG_UART_log(...)
#endif

#define SCL_PIN GIO_GPIO_9
#define SDA_PIN GIO_GPIO_10
#define ADDRESS 0x50

void setup_peripherals_i2c_pin(void)
{
    SYSCTRL_ClearClkGateMulti((1 << SYSCTRL_ITEM_APB_I2C0) | (1 << SYSCTRL_ITEM_APB_PinCtrl));
    PINCTRL_SelI2cIn(I2C_PORT_0, SCL_PIN, SDA_PIN);
}

void setup_peripherals_i2c_module(void)
{
    I2C_Config(APB_I2C0, I2C_ROLE_SLAVE, I2C_ADDRESSING_MODE_07BIT, ADDRESS>>1);
    I2C_ConfigClkFrequency(APB_I2C0, I2C_CLOCKFREQUENY_FASTMODE_PLUS);
    I2C_Enable(APB_I2C0, 1);
    I2C_IntEnable(APB_I2C0, (1 << I2C_INT_CMPL) | (1 << I2C_INT_FIFO_FULL) | (1 << I2C_INT_ADDR_HIT));
}

static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
  
    NVIC_InitStructure.NVIC_IRQChannel =  I2C0_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = 1;
    /* init NVIC */
    NVIC_Init(&NVIC_InitStructure);
}

void I2C0_IRQHandler(void)
{
    uint8_t data;
    uint32_t status = I2C_GetIntState(APB_I2C0);

    if (status & (1 << I2C_STATUS_ADDRHIT))
    {
        I2C_ClearIntState(APB_I2C0, (1 << I2C_STATUS_ADDRHIT));
    }

    if (status & (1 << I2C_STATUS_FIFO_FULL))
    {
        for (int i = 0; i < 4; i++)
        {
            data = I2C_DataRead(APB_I2C0);
            DEBUG_LOG(">>> %02X\n", data);
        }
        
    }

    if (status & (1 << I2C_STATUS_CMPL))
    {
        while (!I2C_FifoEmpty(APB_I2C0))
        {
            data = I2C_DataRead(APB_I2C0);
            DEBUG_LOG("> %02X\n", data);
        }
        

        I2C_ClearIntState(APB_I2C0, (1 << I2C_STATUS_CMPL));
    }

}

uint8_t write_data[8] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};

void i2c_test(void)
{
    
    setup_peripherals_i2c_pin();
    setup_peripherals_i2c_module();
    NVIC_Configuration();
    while (1)
    {
        ;
    }
}
