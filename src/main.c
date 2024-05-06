#include <stdio.h>
#include <string.h>
#include "ingsoc.h"
#include "timer_delay.h"
#include "gpio.h"
#include "uart.h"


#define MAIN_DEBUG
#ifdef MAIN_DEBUG
#include "SEGGER_RTT.h"
#define DEBUG_LOG(...)      SEGGER_RTT_printf(0,##__VA_ARGS__)
#else
#define DEBUG_LOG(...)
#endif

uint32_t cb_putc(char *c, void *dummy)
{
//    while (apUART_Check_TXFIFO_FULL(PRINT_PORT) == 1);
//    UART_SendData(PRINT_PORT, (uint8_t)*c);
    return 0;
}

int fputc(int ch, FILE *f)
{
    cb_putc((char *)&ch, NULL);
    return ch;
}


void HardFault_Handler(void)
{
    while(1)
    {
        ;
    }
}


volatile uint8_t timeout_cnt = 0;
uint8_t GPIO_value = 0,recive_data = 0X39;
static uint8_t toggle_flg = 0;
char test_send_data[20] = {0x36};
int main()
{
    SEGGER_RTT_Init();
    NVIC_SetPriorityGrouping(0x500);
    DEBUG_LOG("RTT init\r\n");
    SysTick_Config(24000);
    NVIC_SetPriority (SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL);

//    gpio_init();
    uart_init_board();
    for(;;)
    {
//        #if defined(TEST_GPIO_OUTPUT)
//        if(toggle_flg)
//            toggle_flg = 0;
//        else
//            toggle_flg = 1;
//        for(uint8_t i = 0;i<3;i++)
//        {   
//            GIO_WriteValue(i,toggle_flg);
//        DEBUG_LOG("get gpio val pin %d :%d\r\n",i,GPIO_value);
//        }
//        for(uint8_t i = 5;i<42;i++)
//        {
//            GIO_WriteValue(i,toggle_flg);
//        DEBUG_LOG("get gpio val pin %d :%d\r\n",i,GPIO_value);
//        }
//        #elif defined(TEST_GPIO_INPUT)
//        for(uint8_t i = 0;i<3;i++)
//        {   
//        GPIO_value = GIO_ReadValue(i);
//        DEBUG_LOG("get gpio val pin %d :%d\r\n",i,GPIO_value);
//        }
//        for(uint8_t i = 5;i<42;i++)
//        {
//        GPIO_value = GIO_ReadValue(i);
//        DEBUG_LOG("get gpio val pin %d :%d\r\n",i,GPIO_value);
//        }
//        #endif
//        recive_data = uart_read_test();
//        uart_send_test((const char*)&recive_data, 1);
        UART_SendData(APB_UART0,0x52);
//        DEBUG_LOG("uart send data :%d\r\n",recive_data);

//        GPIO_value = GIO_ReadValue(GPIO_LED);
//        DEBUG_LOG("get gpio val :%d\r\n",GPIO_value);
//        GPIO_value = GIO_ReadValue(GIO_GPIO_16);;
        SYSCTRL_DelayCycles(1000,1000);
    }
}
