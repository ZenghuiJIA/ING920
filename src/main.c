#include <stdio.h>
#include <string.h>
#include "ingsoc.h"
#include "timer_delay.h"
#include "gpio.h"
#include "uart.h"
#include "test.h"
#include "timer.h"


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


uint8_t test_send[10] = {1,2,3,4,5,6,7,8,9};
int main()
{
    SEGGER_RTT_Init();
    NVIC_SetPriorityGrouping(0x500);
    DEBUG_LOG("RTT init\r\n");
    SysTick_Config(24000);
    NVIC_SetPriority (SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL);
//    uart_init_board();
//    uart_send_test(test_send,sizeof (test_send));
//    watchdog_test();
    timer_pem_test();
    for(;;)
    {
        SYSCTRL_DelayCycles(1000,1000);
    }
}
