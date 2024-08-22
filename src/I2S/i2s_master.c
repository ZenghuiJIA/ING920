#include "i2s_master.h"
#include "misc.h"

//recive only 4depth fifo

#if 1
#include "SEGGER_RTT.h"
#include "debug_print.h"
#define DEBUG_LOG(...)      SEGGER_RTT_printf(0,##__VA_ARGS__)

#define DEBUG_UART_log      debug_uart
#else
#define DEBUG_LOG(...)
#define DEBUG_UART_log(...)
#endif

void I2sGpioInit(void)
{
    // pinctrl & GPIO mux
    PINCTRL_SetPadMux(GIO_GPIO_9, IO_SOURCE_I2S_BCLK_OUT);
    PINCTRL_SetPadMux(GIO_GPIO_10, IO_SOURCE_I2S_DATA_OUT);
    PINCTRL_SelI2sIn(IO_NOT_A_PIN, IO_NOT_A_PIN, GIO_GPIO_10);
    PINCTRL_SetPadMux(GIO_GPIO_11, IO_SOURCE_I2S_LRCLK_OUT);
}

static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
  
    NVIC_InitStructure.NVIC_IRQChannel = I2S_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = 1;
    /* init NVIC */
    NVIC_Init(&NVIC_InitStructure);
}

void I2sConfigInit(void)
{
    // CLK & Register
    SYSCTRL_ConfigPLLClk(6, 128, 2); // sorce clk PLL = 256MHz
    SYSCTRL_SelectI2sClk(SYSCTRL_CLK_PLL_DIV_1 + 4); // I2s_Clk = 51.2MHz
    I2S_ConfigClk(APB_I2S, 25, 32); // F_bclk = 1.024MHz, F_lrclk = 16K
    I2S_Enable(APB_I2S,1,1);
    I2S_ConfigIRQ(APB_I2S, 0, 1, 0, 6);
//    I2S_DMAEnable(APB_I2S, 0, 0);    
    I2S_Config(APB_I2S, I2S_ROLE_MASTER, I2S_MODE_STANDARD, 0, 0, 0, 1, 24);
}

void I2sRecive(void)
{
    I2S_ClearRxFIFO(APB_I2S);
    I2S_Enable(APB_I2S, 0, 1);
}

uint8_t  sendSize = 10;
uint32_t sendData[10];
void I2sSend(void)
{
    int i; 
    I2S_ClearTxFIFO(APB_I2S);
    memset(sendData,0x55,sizeof(sendData));
    // push data into TX_FIFO first
    for (i = 0; i < sendSize; i++) {
        I2S_PushTxFIFO(APB_I2S, sendData[i]);
    }
    I2S_Enable(APB_I2S, 1, 1);
}


void I2S_IRQHandler(void)
{
    uint32_t state,count,data;
    state = I2S_GetIntState(APB_I2S);
    I2S_ClearIntState(APB_I2S,state);
    count = I2S_GetRxFIFOCount(APB_I2S);
    for(uint8_t i = 0;i<count;i++)
    {
        data = I2S_PopRxFIFO(APB_I2S);
        DEBUG_LOG("data:0x%x\r\n",data);
    }
    
    
}


void I2sTest(void)
{
    I2sGpioInit();
    I2sConfigInit();
    NVIC_Configuration();
    I2sSend();
//    I2sRecive();
    while (1)
    {
        ;
    }
}
