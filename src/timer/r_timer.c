#include "ingsoc.h"
#include "misc.h"

#if 1
#include "SEGGER_RTT.h"
#include "debug_print.h"
#define DEBUG_LOG(...)      SEGGER_RTT_printf(0,##__VA_ARGS__)
#define DEBUG_UART_log      debug_uart
#else
#define DEBUG_LOG(...)
#define DEBUG_UART_log
#endif

void rtimer_set_int(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = RCMFDTRIM_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = 1;
    /* init NVIC */
    NVIC_Init(&NVIC_InitStructure);
}

void rtimer_init(void)
{
    SYSCTRL_ClearClkGateMulti(1 << SYSCTRL_ClkGate_APB_RTMR2);
    RTMR_SetCMP(APB_TMR2, 24000000);
    RTMR_SetOpMode(APB_TMR2, TMR_CTL_OP_MODE_WRAPPING);
    RTMR_Reload(APB_TMR2);
    RTMR_IntEnable(APB_TMR2);
}

void rtimer_test(void)
{
    DEBUG_LOG("rtimer int\r\n");
    rtimer_set_int();
    rtimer_init();
    DEBUG_LOG("rtimer int\r\n");
    RTMR_Enable(APB_TMR2);
    while (1)
    {
        ;
    }
    
}

void RCMFDTRIM_IRQHandler(void)
{
    if(RTMR_IntHappened(APB_TMR2))
    {
        DEBUG_LOG("rtimer int\r\n");
        RTMR_IntClr(APB_TMR2);
    }
    
}
