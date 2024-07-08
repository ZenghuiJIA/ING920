#include "debug_print.h"
#include "uart.h"
#include "ano_protocol.h"

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

void debug_hex(void* data, uint32_t len)
{
    write_uart_buffer((char*)data, len);
}

uint32_t debug_len,i_num;
uint8_t ano_buf[256]; 
uint16_t test_data = 0xaabb;

const uint16_t g_SineWave128[] = { 
    2047, 2147, 2248, 2347, 2446, 2544, 2641, 2737, 2830, 2922, 3012, 3099, 3184, 3266, 3346, 3422,  
3494, 3564, 3629, 3691, 3749, 3803, 3852, 3897, 3938, 3974, 4006, 4033, 4055, 4072, 4084, 4092,  
4094, 4092, 4084, 4072, 4055, 4033, 4006, 3974, 3938, 3897, 3852, 3803, 3749, 3691, 3629, 3564,  
3494, 3422, 3346, 3266, 3184, 3099, 3012, 2922, 2830, 2737, 2641, 2544, 2446, 2347, 2248, 2147,  
2047, 1947, 1846, 1747, 1648, 1550, 1453, 1357, 1264, 1172, 1082,  995,  910,  828, 748,   672,  
 600,  530,  465,  403,  345,  291,  242,  197,  156,  120,   88,   61,   39,   22,  10,     2,  
   0,    2,   10,   22,   39,   61,   88,  120,  156,  197,  242,  291,  345,  403, 465,   530,  
 600,  672,  748,  828,  910,  995, 1082, 1172, 1264, 1357, 1453, 1550, 1648, 1747, 1846, 1947,  
}; 

void test_ano(void)
{
    debug_len = packed_ano(ano_buf,(uint8_t*)&g_SineWave128[(i_num++)%128],2);
    debug_hex(ano_buf,debug_len);
}

void ano_output(void* data, uint32_t len)
{
    debug_len = packed_ano(ano_buf,(uint8_t*)data,len);
    debug_hex(ano_buf,debug_len);
}

