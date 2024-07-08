#ifndef _GPIO_H_
#define _GPIO_H_
#include "ingsoc.h"
#include "peripheral_gpio.h"
#include "peripheral_sysctrl.h"
#include "peripheral_pinctrl.h"

//#define TEST_GPIO_OUTPUT
#define TEST_GPIO_INPUT

#define GPIO_LED        GIO_GPIO_9

void gpio_init(void);

#endif
