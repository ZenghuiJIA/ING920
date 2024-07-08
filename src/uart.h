#ifndef __BSP_UART__H__
#define __BSP_UART__H__
#include "ingsoc.h"
#include "peripheral_uart.h"
#include "peripheral_sysctrl.h"
#include "peripheral_pinctrl.h"
#include "peripheral_gpio.h"
#include "peripheral_sysctrl.h"
#include "peripheral_dma.h"

#include <string.h>

void uart_init_board(void);
void uart_send_test(const char *data, int len);
uint8_t uart_read_test(void);
void uart_test_fifo_recive(void);
void uart_test_dma_recive(void);
void all_send_test_data(void);
#endif
