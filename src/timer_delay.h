#ifndef _TIMER_DELAY_H_
#define _TIMER_DELAY_H_

#include "ingsoc.h"

extern __IO uint32_t jiffies;

#define time_after(a,b)         \
         ((long)((b) - (a)) < 0)
#define time_before(a,b)        time_after(b,a)

#define time_after_eq(a,b)      \
         ((long)((a) - (b)) >= 0)
#define time_before_eq(a,b)     time_after_eq(b,a)

#define wait_for_condition(a,b)      wait_for_condition_func(a,b)

#define wait_for_condition_func(condition, timeout_ms, func) ({        \
    uint32_t __start = jiffies;                        \
    int __ret = -1;                     \
    while ((condition) && time_before_eq(jiffies, __start + (timeout_ms))) { \
        if(condition)   {\
            func;\
        }\
    }                                                       \
    if (!(condition)) __ret = 0;                             \
    __ret;                                                  \
})

void delay_ms(uint32_t delay);
#endif
