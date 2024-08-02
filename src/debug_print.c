#include "debug_print.h"
#include "uart.h"
#include "ano_protocol.h"
#include "SEGGER_RTT.h"
#define BDS_COLOR_TAG           "BDSCOL"

#define BDS_LOG_COLOR_BLACK     0x000000 //黑色
#define BDS_LOG_COLOR_RED       0xFF3030 //共色
#define BDS_LOG_COLOR_GREEN     0x008B00 //绿色
#define BDS_LOG_COLOR_BLUE      0x00008B //蓝色
#define BDS_LOG_COLOR_VIOLET    0x9932CC //紫色

//char *s = "test\n";

////红色
//SEGGER_RTT_printf(0, BDS_COLOR_TAG "(%d)%s", BDS_LOG_COLOR_RED, s);
////绿色
//SEGGER_RTT_printf(0, BDS_COLOR_TAG "(%d)%s", BDS_LOG_COLOR_GREEN, s);
////黑色
//SEGGER_RTT_printf(0, BDS_COLOR_TAG "(%d)%s", BDS_LOG_COLOR_BLACK, s);
////黑色。不带BDS_COLOR_TAG标签时，RTT_T2按照默认按照黑色显示
//SEGGER_RTT_printf(0,"%s", s);
////蓝色
//SEGGER_RTT_printf(0, BDS_COLOR_TAG "(%d)%s", BDS_LOG_COLOR_BLUE, s);
////紫色
//SEGGER_RTT_printf(0, BDS_COLOR_TAG "(%d)%s", BDS_LOG_COLOR_VIOLET, s);


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

void rtt_printf_int_test(void)
{
    static uint32_t sn = 0;
    int32_t X=-100,Y=105,Z=20;
    int32_t x,y,z;

    x = X;
    y = Y;
    z = Z;
    SEGGER_RTT_printf(0,  BDS_COLOR_TAG"(%d)TAG=DLOG SN(%d)M*%d(%d,%d,%d)\n", BDS_LOG_COLOR_RED, sn++, 0, x, y, z);

}

void rtt_line_int16(int16_t data)
{
    static uint32_t sn = 0;

    SEGGER_RTT_printf(0,  BDS_COLOR_TAG"(%d)TAG=DLOG SN(%d)M*%d(%d,%d,%d)\n", BDS_LOG_COLOR_RED, sn++, 0, data, 0, 0);
}

void rtt_printf_float_test(void)
{
    static uint32_t sn = 0;
    static float X=0.123,Y=5.638941,Z=9.8874;
    int32_t x,y,z;

    x = X * 10000;
    y = Y * 10000;
    z = Z * 10000;
    //保留1位小数点
    SEGGER_RTT_printf(0,  BDS_COLOR_TAG"(%d)TAG=DLOG SN(%d)M*%d(%d,%d,%d)\n", BDS_LOG_COLOR_RED, sn++, 4, x, y, z);

}

