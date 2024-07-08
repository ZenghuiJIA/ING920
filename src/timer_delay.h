#ifndef _TIMER_DELAY_H_
#define _TIMER_DELAY_H_

#include "ingsoc.h"

#define time_after(a,b)         \
         ((long)((b) - (a)) < 0)
#define time_before(a,b)        time_after(b,a)

#define time_after_eq(a,b)      \
         ((long)((a) - (b)) >= 0)
#define time_before_eq(a,b)     time_after_eq(b,a)

extern __IO uint32_t jiffes;

#endif
