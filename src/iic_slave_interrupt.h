#ifndef _IIC_SLAVE_INTERRUPT_H_
#define _IIC_SLAVE_INTERRUPT_H_
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "peripheral_i2c.h"
#include "platform_api.h"

/*
iic slave address.If you include more than one slave, re-initialize 
the I2C_Config function to change address before you send data.
*/
#define ADDRESS (0x71)

#define IIC_PORT    I2C_PORT_0
#define IIC_BASE    APB_I2C0
#define IIC_SCL_PIN GIO_GPIO_7
#define IIC_SDA_PIN GIO_GPIO_8

//please check you need send max data len.
#define READ_DATA_LEN   8
#define WRITE_DATA_LEN  20

typedef enum {
    iic_write,
    iic_read,
}WR_STATE;

void iic_init(void);

#endif
