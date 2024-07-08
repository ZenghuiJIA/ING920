
#ifndef ING920_SDK_DEBUG_PRINT_H
#define ING920_SDK_DEBUG_PRINT_H
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

void debug_uart(const char *fmt, ...);
void debug_hex(void* data, uint32_t len);
void test_ano(void);
void ano_output(void* data, uint32_t len);
#endif //ING920_SDK_DEBUG_PRINT_H
