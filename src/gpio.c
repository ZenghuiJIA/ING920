#include "gpio.h"
#include "misc.h"

#define GPIO_DEBUG
#ifdef GPIO_DEBUG
#include "SEGGER_RTT.h"
#define DEBUG_LOG(...)      SEGGER_RTT_printf(0,##__VA_ARGS__)
#else
#define DEBUG_LOG(...)
#endif

#if defined(TEST_GPIO_OUTPUT)
void gpio_init(void)
{
    //enable peripheral sysctrl
    SYSCTRL_ClearClkGate(SYSCTRL_ClkGate_APB_GPIO0);//gpio0
    SYSCTRL_ClearClkGate(SYSCTRL_ClkGate_APB_GPIO1);//gpio1

    for(uint8_t i = 0;i<3;i++)
    {
        PINCTRL_SetPadMux(i, IO_SOURCE_GPIO);
        GIO_SetDirection((GIO_Index_t)i,GIO_DIR_OUTPUT);
        //normal mode is enabled
        GIO_WriteValue((GIO_Index_t)i, 0);
    }
    for(uint8_t i = 5;i<41;i++)
    {
        PINCTRL_SetPadMux(i, IO_SOURCE_GPIO);
        GIO_SetDirection((GIO_Index_t)i,GIO_DIR_OUTPUT);
        GIO_WriteValue((GIO_Index_t)i, 0);
    }
    
}
void gpio_toogle(uint8_t bit)
{
    static uint8_t i = 0;
    if(i)
    {
        i = 0;
        GIO_WriteValue((GIO_Index_t)bit, i);
    }
    else
    {
        i = 1;
        GIO_WriteValue((GIO_Index_t)bit, i);
    }
}
#elif defined(TEST_GPIO_INPUT)

void GPIO0_IRQHandler(void)
{
    if(GIO_GetIntStatus(GPIO_LED))
    {
        DEBUG_LOG("int gpio0 tigger\r\n");
        GIO_ClearIntStatus(GPIO_LED);
    }
}

static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
  
    NVIC_InitStructure.NVIC_IRQChannel = GPIO0_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = 1;
    /* init NVIC */
    NVIC_Init(&NVIC_InitStructure);
}

void gpio_init(void)
{
    //enable peripheral sysctrl
    SYSCTRL_ClearClkGate(SYSCTRL_ClkGate_APB_GPIO0);//gpio0
    SYSCTRL_ClearClkGate(SYSCTRL_ClkGate_APB_GPIO1);//gpio1
    NVIC_Configuration();
    for(uint8_t i = 0;i<3;i++)
    {
        PINCTRL_SetPadMux(i, IO_SOURCE_GPIO);
        GIO_SetDirection((GIO_Index_t)i,GIO_DIR_INPUT);
    }
    for(uint8_t i = 5;i<41;i++)
    {
        PINCTRL_SetPadMux(i, IO_SOURCE_GPIO);
        GIO_SetDirection((GIO_Index_t)i,GIO_DIR_INPUT);
    }
    

    #if 0
    //enable interrupt falling edge
    GIO_ConfigIntSource(GIO_GPIO_9,GIO_INT_EN_LOGIC_LOW_OR_FALLING_EDGE,GIO_INT_EDGE);
    GIO_ClearIntStatus(GPIO_LED);
    NVIC_Configuration();
    
    #endif
}
#endif
