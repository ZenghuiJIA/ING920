#ifndef __IIC_MASTER_INTERRUPT_H_
#define __IIC_MASTER_INTERRUPT_H_
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
#define DATA_CNT (8)

typedef enum {
    iic_write,
    iic_read,
}WR_STATE;

void iic_init(void);
int peripheral_i2c_send_data(const void* buf,uint8_t data_len);
int peripheral_i2c_read_data(const void* buf,uint8_t data_len);

#endif
