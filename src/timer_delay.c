#include "timer_delay.h"

__IO uint32_t jiffes = 0;

void SysTick_Handler(void)
{
    jiffes++;
}
