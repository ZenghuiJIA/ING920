#include "board.h"
#include "ingsoc.h"
#include "platform_api.h"
#include "FreeRTOS.h"
#include "peripheral_i2c.h"
#include "iic_master_interrupt.h"
#include "string.h"

void led_init(void)
{ 
    SYSCTRL_ClearClkGateMulti((1 << SYSCTRL_ClkGate_APB_GPIO0));
    PINCTRL_SetPadMux(GIO_GPIO_14, IO_SOURCE_GPIO);

    GIO_SetDirection(GIO_GPIO_14, GIO_DIR_OUTPUT);
    GIO_WriteValue(GIO_GPIO_14, 0);
}
void key_init(void)
{ 
    SYSCTRL_ClearClkGateMulti((1 << SYSCTRL_ClkGate_APB_GPIO1));
    PINCTRL_SetPadMux(GIO_GPIO_26, IO_SOURCE_GPIO);

    GIO_SetDirection(GIO_GPIO_26, GIO_DIR_INPUT);
}

//#define INT_IIC
////io7 io8
//void iic_init(void)
//{
//    SYSCTRL_ClearClkGateMulti((1 << SYSCTRL_ClkGate_APB_GPIO0)|(1 << SYSCTRL_ClkGate_APB_I2C0));
//    PINCTRL_SelI2cIn(I2C_PORT_0,GIO_GPIO_7,GIO_GPIO_8);
//#ifdef INT_IIC
//    platform_set_irq_callback(PLATFORM_CB_IRQ_I2C0, peripherals_i2c_isr, NULL);
//    setup_peripherals_i2c_module();
//#else
//    i2c_init(I2C_PORT_0);
//#endif
//}


void uart1_init(void)
{
    UART_sStateStruct uart_init_strucet;

    SYSCTRL_ClearClkGateMulti((1 << SYSCTRL_ClkGate_APB_UART1)|(1 << SYSCTRL_ClkGate_APB_GPIO0));

    uart_init_strucet.BaudRate = 115200;
    uart_init_strucet.ClockFrequency = OSC_CLK_FREQ;
    uart_init_strucet.cts_en = 0;
    uart_init_strucet.fifo_enable = 1;
    uart_init_strucet.parity = UART_PARITY_NOT_CHECK;
    uart_init_strucet.receive_en = 1;
    uart_init_strucet.rts_en = 0;
    uart_init_strucet.rxfifo_waterlevel = 1;
    uart_init_strucet.transmit_en = 1;
    uart_init_strucet.two_stop_bits = 0;
    uart_init_strucet.txfifo_waterlevel = 1;
    uart_init_strucet.UART_en = 1;
    uart_init_strucet.word_length = UART_WLEN_8_BITS;

    apUART_Initialize(APB_UART1, &uart_init_strucet,0);

    PINCTRL_SetPadMux(GIO_GPIO_35,IO_SOURCE_UART1_TXD);
    
}

void test_input_init(void)
{ 
    SYSCTRL_ClearClkGateMulti((1 << SYSCTRL_ClkGate_APB_GPIO0));
    PINCTRL_SetPadMux(GIO_GPIO_18, IO_SOURCE_GPIO);
    PINCTRL_SetPadMux(GIO_GPIO_17, IO_SOURCE_GPIO);
    GIO_SetDirection(GIO_GPIO_18, GIO_DIR_INPUT);
    GIO_SetDirection(GIO_GPIO_17, GIO_DIR_NONE);
}

static uint32_t ADC_cb_isr(void *user_data)
{
    uint32_t data = ADC_PopFifoData();
    SADC_channelId channel = ADC_GetDataChannel(data);
    if (channel == ADC_CH_2) {
        uint16_t sample = ADC_GetData(data);
        platform_printf("GET POP fifo data = %x\r\n",data);
        platform_printf("GET ADC VALE adc_data = %x\r\n",sample);
        // do something with 'sample'
    }
    ADC_Start(0);
    return 0;
}

static uint32_t rtc_isr(void *user_data)
{
    uint8_t time_h,time_m,time_s;
    
    RTC_ClearIntState(RTC_IRQ_SECOND);
    RTC_GetTime(&time_h,&time_m,&time_s);
    
    //platform_printf("Now get rtc time %d:%d:%d\r\n",time_h,time_m,time_s);
    return 0;
}

void adc_init(void)
{
    SYSCTRL_ClearClkGate(SYSCTRL_ITEM_APB_ADC);
//    PINCTRL_EnableAnalog(9);
//    PINCTRL_SetPadMux(9, 0);
//    GIO_SetDirection((GIO_Index_t)9, GIO_DIR_NONE);
    SYSCTRL_SetAdcClkDiv(24 / 6);
    SYSCTRL_ReleaseBlock(SYSCTRL_ITEM_APB_ADC);
    ADC_Reset();
    ADC_ftInit();
    ADC_Calibration(SINGLE_END_MODE);
    ADC_ConvCfg(SINGLE_MODE, PGA_PARA_1, 0, ADC_CH_2, 1, 0, SINGLE_END_MODE, 0);
    platform_set_irq_callback(PLATFORM_CB_IRQ_SADC, ADC_cb_isr, 0);
    platform_set_irq_callback(PLATFORM_CB_IRQ_RTC, (f_platform_irq_cb)rtc_isr, NULL);
    ADC_Start(1);
   
}
void board_init(void)
{
    led_init();
    key_init();
    adc_init();
    uart1_init();
    iic_init();
//    test_input_init();
}
