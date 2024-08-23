#include "iic_master.h"
#include "peripheral_gpio.h"
#include "peripheral_sysctrl.h"


#if 1
#include "SEGGER_RTT.h"
#include "debug_print.h"
#define DEBUG_LOG(...)      SEGGER_RTT_printf(0,##__VA_ARGS__)

#define DEBUG_UART_log      debug_uart
#else
#define DEBUG_LOG(...)
#define DEBUG_UART_log(...)
#endif


#define SCL_PIN GIO_GPIO_9
#define SDA_PIN GIO_GPIO_10
#define ADDRESS 0x50

uint8_t crc8_calculate(uint8_t *data, uint8_t len)
{
    uint8_t crc = 0xFF;  // 初始值为0xFF
    uint8_t poly = 0x31; // 多项式x^8 + x^5 + x^4 + 1 对应0x31

    for (uint8_t i = 0; i < len; i++)
    {
        crc ^= data[i]; // 将当前字节与CRC寄存器的值异或
        for (uint8_t j = 0; j < 8; j++)
        {
            if (crc & 0x80)
            {
                crc = (crc << 1) ^ poly; // 左移并异或多项式
            }
            else
            {
                crc <<= 1; // 直接左移
            }
        }
    }

    crc ^= 0x00; // 计算结果与0x00异或（此操作对结果没有实际影响，但根据要求执行）

    return crc;
}

void setup_peripherals_i2c_pin(void)
{
    SYSCTRL_ClearClkGateMulti((1 << SYSCTRL_ITEM_APB_I2C0) | (1 << SYSCTRL_ITEM_APB_PinCtrl));
    PINCTRL_SelI2cIn(I2C_PORT_0, SCL_PIN, SDA_PIN);
}

void setup_peripherals_i2c_module(void)
{
    I2C_Config(APB_I2C0, I2C_ROLE_MASTER, I2C_ADDRESSING_MODE_07BIT, ADDRESS);
    I2C_ConfigClkFrequency(APB_I2C0, I2C_CLOCKFREQUENY_STANDARD);
    I2C_Enable(APB_I2C0, 1);
    // I2C_IntEnable(APB_I2C0,(1<<I2C_INT_CMPL)|(1 << I2C_INT_FIFO_FULL));
}

// static uint32_t peripherals_i2c_isr(void *user_data)
// {
//   uint32_t status = I2C_GetIntState(APB_I2C0);

//   if(status & (1 << I2C_STATUS_FIFO_FULL))
//   {
//     for(; read_data_cnt < DATA_CNT; read_data_cnt++)
//     {
//       if(I2C_FifoEmpty(APB_I2C0)){ break; }
//       read_data[read_data_cnt] = I2C_DataRead(APB_I2C0);
//     }
//   }

//   if(status & (1 << I2C_STATUS_CMPL))
//   {
//     for(; read_data_cnt < DATA_CNT; read_data_cnt++)
//     {
//       read_data[read_data_cnt] = I2C_DataRead(APB_I2C0);
//     }
//     I2C_ClearIntState(APB_I2C0, (1 << I2C_STATUS_CMPL));
//   }

//   return 0;
// }

uint8_t write_data[8] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};

void i2c_test(void)
{
    uint8_t data;
    setup_peripherals_i2c_pin();
    setup_peripherals_i2c_module();
//    I2C_CtrlUpdateDirection(APB_I2C0,I2C_TRANSACTION_MASTER2SLAVE);
//    I2C_CtrlUpdateDataCnt(APB_I2C0,8);
//    I2C_CommandWrite(APB_I2C0,0x1);
//    for(int i = 0;i < 8;i++)
//    {
//        while(I2C_FifoFull(APB_I2C0)) ;
//        I2C_DataWrite(APB_I2C0,write_data[i]);
//    }
    I2C_CtrlUpdateDirection(APB_I2C0,I2C_TRANSACTION_SLAVE2MASTER);
    I2C_CtrlUpdateDataCnt(APB_I2C0,4);
    I2C_CommandWrite(APB_I2C0,0x1);
    for(int i = 0;i < 8;i++)
    {
        while(I2C_FifoEmpty(APB_I2C0)) ;
        data = I2C_DataRead(APB_I2C0);
        DEBUG_LOG("revive data:0x%x\r\n",data);
    }
    while (1)
    {
        ;
    }
}
