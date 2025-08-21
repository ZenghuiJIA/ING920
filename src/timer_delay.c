#include "timer_delay.h"
#include "stdio.h"

__IO uint32_t jiffies = 0;

//void SysTick_Handler(void)
//{
//    jiffies++;
//    // printf("jiffes: %d\n",jiffes);
//}

void delay_ms(uint32_t delay) {
    uint32_t timeout = jiffies + (delay);

    while (time_before(jiffies, timeout)) {
        // 等待，直到 jiffies 达到 timeout 的值
    }
}

uint32_t get_jiffies(void) {
    return  jiffies;
}
