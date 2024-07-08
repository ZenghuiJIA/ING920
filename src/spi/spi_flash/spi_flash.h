#ifndef  _SPI_FLASH_H_
#define  _SPI_FLASH_H_

#include "ingsoc.h"
#include "peripheral_gpio.h"
#include "peripheral_sysctrl.h"
#include "peripheral_pinctrl.h"
#include "peripheral_ssp.h"
#include "stdio.h"

#define COMMAND_W_ENABLE         0x06
#define COMMAND_W_DISABLE        0x04
#define COMMAND_ENABLE_V_STATUS  0x50
#define COMMAND_READ_STATUS      0x05
#define	COMMAND_WRITE_STATUS     0x01
#define COMMAND_READ_DATA        0x03
#define COMMAND_FAST_READ        0x0B
#define COMMAND_PAGE_PROGRAM     0x02
#define COMMAND_SECTOR_ERASE     0x20
#define COMMAND_BLOCK_ERASE      0xD8
#define COMMAND_CHIP_ERASE       0xC7
#define COMMAND_READ_ID          0x9F
#define COMMAND_READ_DEVICE_ID   0x90

void spi_test(void);

#endif
