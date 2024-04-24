#include <stdio.h>
#include <string.h>
#include "ingsoc.h"

uint32_t cb_putc(char *c, void *dummy)
{
//    while (apUART_Check_TXFIFO_FULL(PRINT_PORT) == 1);
//    UART_SendData(PRINT_PORT, (uint8_t)*c);
    return 0;
}

int fputc(int ch, FILE *f)
{
    cb_putc((char *)&ch, NULL);
    return ch;
}

int chip_init(void)
{
    return 1;
}

int main()
{
    for(;;)
    {
        
    }
    return 0;
}

