#include "debug_print.h"
#include "uart.h"

__weak void write_uart_buffer(const char *buffer, int len)
{
    uart_send_test(buffer,len);
}
// 打印到串口
void debug_uart(const char *fmt, ...)
{
    uint32_t length;
    va_list args;
    char TxBuffer[128] = {0};

    va_start(args, fmt);
    length = vsnprintf((char*)TxBuffer, sizeof(TxBuffer), (char*)fmt, args);
    va_end(args);

    write_uart_buffer(TxBuffer, length);
    
}