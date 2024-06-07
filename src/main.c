#include <stdio.h>
#include <string.h>
#include "ingsoc.h"
#include "timer_delay.h"
#include "gpio.h"
#include "uart.h"
#include "test.h"
#include "timer.h"
#include "PWM.h"
#include "spi_master.h"
#include "iic_master_920.h"
#include "keyscan.h"
#include "qdec.h"

#define MAIN_DEBUG
#ifdef MAIN_DEBUG
#include "SEGGER_RTT.h"
#include "debug_print.h"
#define DEBUG_LOG(...)      SEGGER_RTT_printf(0,##__VA_ARGS__)
#define DEBUG_UART_log      debug_uart
#else
#define DEBUG_LOG(...)
#define DEBUG_UART_log(...)
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
    __disable_irq();
    SCB->VTOR = 0x2000;
    __enable_irq();
    SEGGER_RTT_Init();
    NVIC_SetPriorityGrouping(0x500);
    DEBUG_LOG("RTT init\r\n");
    SysTick_Config(24000);
    NVIC_SetPriority (SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL);
    uart_init_board();
    debug_uart("debug uart_test\r\n");
    qudec_test();
//    key_scan_normal_test();
//    IR_test_cycle();
//    __enable_irq();
//    all_send_test_data();
//    iic_test();
//    spi_test();
//    pwm_test_simple();
//    pwm_test_step();
//    uart_test_dma_recive();
//    pcap_test_read();

//    uart_send_test(test_send,sizeof (test_send));
//    watchdog_test();
//    timer_pem_test();
    for(;;)
    {
        SYSCTRL_DelayCycles(1000,1000);
    }
}
