#include "timer.h"
#include "misc.h"
#include "peripheral_timer.h"
#include "peripheral_sysctrl.h"
#include "peripheral_pinctrl.h"
#include "peripheral_uart.h"

#define TIMER_DEBUG
#ifdef TIMER_DEBUG
#include "SEGGER_RTT.h"
#define DEBUG_LOG(...)      SEGGER_RTT_printf(0,##__VA_ARGS__)
#else
#define DEBUG_LOG(...)
#endif

//32位定时器初始化
void timer_32_init(void)
{
    SYSCTRL_ClearClkGate(SYSCTRL_ClkGate_APB_TMR0);
    //初始化通道模式
    TMR_SetOpMode(APB_TMR0,0,TMR_CTL_OP_MODE_32BIT_TIMER_x1,TMR_CLK_MODE_APB,0);
    TMR_SetReload(APB_TMR0, 0, TMR_GetClk(APB_TMR0, 0)/5);  //4999
    TMR_Enable(APB_TMR0, 0, 0xf);
    TMR_IntEnable(APB_TMR0, 0, 0xf);
}

void timer_32_set_int(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = TIMER0_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = 1;
    /* init NVIC */
    NVIC_Init(&NVIC_InitStructure);
}

void TIMER0_IRQHandler(void)
{
    DEBUG_LOG("Timer0_IRQHandler\r\n");
    TMR_IntClr(APB_TMR0, 0, 0xf);
    TMR_PauseEnable(APB_TMR0,1);
}

void timer_test(void)
{
    timer_32_init();
    timer_32_set_int();
    while(1)
    {
        ;
    }
}

#define PIN_TMR_PWM0 9
#define PIN_TMR_PWM1 10
void timer_pwn_init(void)
{
    SYSCTRL_ClearClkGateMulti( (1 << SYSCTRL_ITEM_APB_SysCtrl)
                               |(1 << SYSCTRL_ITEM_APB_PinCtrl)
                               |(1 << SYSCTRL_ITEM_APB_TMR1));

    TMR_SetOpMode(APB_TMR0, 0, TMR_CTL_OP_MODE_16BIT_PWM, TMR_CLK_MODE_APB, 1);
    TMR_SetOpMode(APB_TMR0, 1, TMR_CTL_OP_MODE_16BIT_PWM, TMR_CLK_MODE_APB, 1);
    TMR_SetReload(APB_TMR0, 0, 0x00090013);   //9   19
    TMR_SetReload(APB_TMR0, 1, 0x00130027);   //19  39
    TMR_Enable(APB_TMR0, 0, 0xf);
    TMR_Enable(APB_TMR0, 1, 0xf);

    PINCTRL_SetPadMux(PIN_TMR_PWM0, IO_SOURCE_TIMER0_PWM0_B);
    PINCTRL_SetPadMux(PIN_TMR_PWM1, IO_SOURCE_TIMER0_PWM1_A);
}

void timer_pem_test(void)
{
    timer_pwn_init();
    while(1)
    {
        ;
    }
}

void watchdog_init(void)
{
    SYSCTRL_ClearClkGate(SYSCTRL_ClkGate_APB_WDT);
    //enable watchdog but disable interrupt
    TMR_WatchDogEnable3(WDT_INTTIME_INTERVAL_1M_4S,WDT_RSTTIME_INTERVAL_500MS,1);
}

void watchdog_set_int(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = WDT_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = 1;
    /* init NVIC */
    NVIC_Init(&NVIC_InitStructure);
}

void WDT_IRQHandler(void)
{
    TMR_WatchDogClearInt();
    UART_SendData(APB_UART0,0xff);
    DEBUG_LOG("WDT IRQ Handler \r\n");
    TMR_WatchDogRestart();
}

void watchdog_test(void)
{
    watchdog_init();
    watchdog_set_int();
    while (1)
    {
        //feed dog
//        TMR_WatchDogRestart();
        DEBUG_LOG("feed dog\r\n");
        SYSCTRL_DelayCycles(1000,1000);
    }
}
