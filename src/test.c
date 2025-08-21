#include "test.h"
#include <stdio.h>
#include <string.h>
#include "ingsoc.h"
#include "timer_delay.h"
#include "gpio.h"
#include "uart.h"
#define TEST_DEBUG
#ifdef TEST_DEBUG
#include "SEGGER_RTT.h"
#define DEBUG_LOG(...)      SEGGER_RTT_printf(0,##__VA_ARGS__)
#else
#define DEBUG_LOG(...)
#endif

volatile uint8_t timeout_cnt = 0;
uint8_t GPIO_value = 0,recive_data = 0X39;
static uint8_t toggle_flg = 0;
char test_send_data[20] = {0x36};
void gpio_output_test(void)
{
    gpio_init();
    while(1)
    {
        #if defined(TEST_GPIO_OUTPUT)
        if(toggle_flg)
            toggle_flg = 0;
        else
            toggle_flg = 1;
        for(uint8_t i = 0;i<3;i++)
        {
            GIO_WriteValue(i,toggle_flg);
        DEBUG_LOG("get gpio val pin %d :%d\r\n",i,GPIO_value);
        }
        for(uint8_t i = 5;i<42;i++)
        {
            GIO_WriteValue(i,toggle_flg);
        DEBUG_LOG("get gpio val pin %d :%d\r\n",i,GPIO_value);
        }
        #elif defined(TEST_GPIO_INPUT)
        for(uint8_t i = 0;i<3;i++)
        {
        GPIO_value = GIO_ReadValue((GIO_Index_t)i);
        DEBUG_LOG("get gpio val pin %d :%d\r\n",i,GPIO_value);
        }
        for(uint8_t i = 5;i<42;i++)
        {
        GPIO_value = GIO_ReadValue((GIO_Index_t)i);
        DEBUG_LOG("get gpio val pin %d :%d\r\n",i,GPIO_value);
        }
        #endif
        SYSCTRL_DelayCycles(1000,1000);
    }
}

void uart_test(void)
{
    uart_init_board();
    while(1) {
        recive_data = uart_read_test();
        uart_send_test((const char*)&recive_data, 1);

        DEBUG_LOG("uart send data :%d\r\n",recive_data);
        apUART_Enable_TRANSMIT_INT(APB_UART0);
        UART_SendData(APB_UART0,0x53);
        UART_SendData(APB_UART0,0x54);

        SYSCTRL_DelayCycles(1000,1000);

    }
}
