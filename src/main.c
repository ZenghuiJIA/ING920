#include <stdio.h>
#include <string.h>
#include "ingsoc.h"
#include "timer_delay.h"
#include "gpio.h"
#include "uart.h"
#include "test.h"
#include "timer.h"
#include "PWM.h"
#include "iic_master_920.h"
#include "keyscan_normal.h"
#include "qdec.h"
#include "spi_normal.h"
#include "PTE_lib_timer.h"
#include "PDM.h"
#include "r_timer.h"
#include "i2s_master.h"


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
    while (apUART_Check_TXFIFO_FULL(APB_UART0) == 1);
    UART_SendData(APB_UART0, (uint8_t)*c);
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

int main()
{
    __disable_irq();
    SCB->VTOR = 0x2002000;
    __enable_irq();
    NVIC_SetPriorityGrouping(0x500);
    
    SysTick_Config(24000);
    
    
    uart_init_board();
    debug_uart("debug uart_test\r\n");
    
    SEGGER_RTT_Init();
//    SEGGER_RTT_Write(1, &g_SineWave128[(i_num++)%128], 2);
    DEBUG_LOG("debug rtt\r\n");
//    pte_test();
//    qudec_test();
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
//    pdm_test();
//    rtimer_test();
//    uart_test_fifo_recive();
//    I2sTest();

//    uart_send_test(test_send,sizeof (test_send));
//    watchdog_test();
//    pte_test();
    i2c_test();
    for(;;)
    {
        SYSCTRL_DelayCycles(1000,1000);
        SYSCTRL_DelayCycles(24000,10000);
    }
}
