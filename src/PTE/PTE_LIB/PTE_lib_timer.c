#include "timer_pte.h"
#include "peripheral_timer.h"
#include "misc.h"

#if 1
#include "SEGGER_RTT.h"
#include "debug_print.h"
#define DEBUG_LOG(...)      SEGGER_RTT_printf(0,##__VA_ARGS__)

#define DEBUG_UART_log      debug_uart
#else
#define DEBUG_LOG(...)
#define DEBUG_UART_log(...)
#endif

uint32_t data = 0x12345679;

void TIMER0_IRQHandler(void)
{
    DEBUG_LOG("timer0 isr\n");
    TMR_Enable(APB_TMR0, 0, 0x0);
//    TMR_Enable(APB_TMR1, 0, 0xf);
    TMR_IntClr(APB_TMR0, 0, 0xf);
}

void TIMER1_IRQHandler(void)
{
    DEBUG_LOG("timer1 isr\n");
////    TMR_Enable(APB_TMR1, 0, 0x0);
    TMR_IntClr(APB_TMR1, 0, 0xf);
}

void timer1_init(void)
{
    SYSCTRL_ClearClkGate(SYSCTRL_ClkGate_APB_TMR1);
    //初始化通道模式
    TMR_SetOpMode(APB_TMR1,0,TMR_CTL_OP_MODE_32BIT_TIMER_x1,TMR_CLK_MODE_APB,0);
    TMR_SetReload(APB_TMR1, 0, TMR_GetClk(APB_TMR1, 0));  //4999
    TMR_Enable(APB_TMR1, 0, 0xf);
    TMR_IntEnable(APB_TMR1, 0, 0xf);
}

void timer0_init(void)
{
    SYSCTRL_ClearClkGate(SYSCTRL_ClkGate_APB_TMR0);
    //初始化通道模式
    TMR_SetOpMode(APB_TMR0,0,TMR_CTL_OP_MODE_32BIT_TIMER_x1,TMR_CLK_MODE_APB,0);
    TMR_SetReload(APB_TMR0, 0, TMR_GetClk(APB_TMR0, 0));  //4999
//    TMR_Enable(APB_TMR0, 0, 0xf);
    TMR_IntEnable(APB_TMR0, 0, 0xf);
}

static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
  
    NVIC_InitStructure.NVIC_IRQChannel = TIMER0_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = 1;
    /* init NVIC */
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = TIMER1_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = 1;
    /* init NVIC */
    NVIC_Init(&NVIC_InitStructure);
}


uint32_t *reg_ptr;
void pte_test(void)
{
    PTE_SetModuleTaskEventConfig(PTE_TIMER0_MODULE,PTE_CHANNEL_0,PTE_TASK_TIMER_CHX_TMRX_ENABLE,0);
    PTE_SetModuleTaskEventConfig(PTE_TIMER1_MODULE,PTE_CHANNEL_0,PTE_EVENT_TIMER_CHX_TMRX,0);
    // APB_PTE->reg_ch_en_set &= 1<<0;
    // reg_ptr = (uint32_t*)0x400181b0;
    // *reg_ptr &= (0x1 << 31);//sub timer0 recive timer1
    // reg_ptr = (uint32_t*)0x40018474;
    // *reg_ptr &= (0x1 << 31);//pub timer1 singnel timer1
    PTE_ChxEnable(1<<PTE_CHANNEL_0);
    timer1_init();
    timer0_init();
    NVIC_Configuration();

    while (1);
}
