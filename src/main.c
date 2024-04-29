#include <stdio.h>
#include <string.h>
#include "ingsoc.h"
#include "timer_delay.h"
#include "gpio.h"


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

void gpio_toogle(uint8_t pin);

volatile uint8_t timeout_cnt = 0;
uint8_t GPIO_value = 0;
int main()
{
    SEGGER_RTT_Init();
    NVIC_SetPriorityGrouping(0x500);
    DEBUG_LOG("RTT init\r\n");
    SysTick_Config(24000);
    NVIC_SetPriority (SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL);

    gpio_init();
    for(;;)
    {
        // gpio_toogle(GPIO_LED);
        GPIO_value = GIO_ReadValue(GPIO_LED);
        DEBUG_LOG("get gpio val :%d\r\n",GPIO_value);
        SYSCTRL_DelayCycles(1000,1000);
    }
}

