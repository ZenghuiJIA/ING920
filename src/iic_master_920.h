#ifndef ING920_SDK_IIC_MASTER_920_H
#define ING920_SDK_IIC_MASTER_920_H
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "peripheral_i2c.h"
#include "peripheral_sysctrl.h"
#include "peripheral_pinctrl.h"
#include "peripheral_gpio.h"
#include "misc.h"

/*
iic slave address.If you include more than one slave, re-initialize
the I2C_Config function to change address before you send data.
*/
#define ADDRESS (0x71)

#define IIC_PORT    I2C_PORT_0
#define IIC_BASE    APB_I2C0
#define IIC_SCL_PIN GIO_GPIO_9
#define IIC_SDA_PIN GIO_GPIO_10

//please check you need send max data len.
#define DATA_CNT (8)

typedef enum {
    iic_write,
    iic_read,
}WR_STATE;

void iic_test(void);
#endif //ING920_SDK_IIC_MASTER_920_H
