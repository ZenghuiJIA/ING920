
#ifndef ING920_SDK_DEBUG_PRINT_H
#define ING920_SDK_DEBUG_PRINT_H
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

void debug_uart(const char *fmt, ...);
void debug_hex(void* data, uint32_t len);
void test_ano(void);
void ano_output(void* data, uint32_t len);
void rtt_line_int16(int16_t data);
void uart_init_board(void);
void delay_ms(uint32_t delay);
uint32_t get_jiffies(void);
void uart_init_board(void);
#endif //ING920_SDK_DEBUG_PRINT_H
