#include "uart.h"
#include "misc.h"
//#define UART_INT

#define UART_DEBUG
#ifdef UART_DEBUG
#include "SEGGER_RTT.h"
#define DEBUG_LOG(...)      SEGGER_RTT_printf(0,##__VA_ARGS__)
#else
#define DEBUG_LOG(...)
#endif

#define bsDMA_INT_C_MASK         1
#define bsDMA_DST_REQ_SEL        4
#define bsDMA_SRC_REQ_SEL        8
#define bsDMA_DST_ADDR_CTRL      12
#define bsDMA_SRC_ADDR_CTRL      14
#define bsDMA_DST_MODE           16
#define bsDMA_SRC_MODE           17
#define bsDMA_DST_WIDTH          18
#define bsDMA_SRC_WIDTH          21
#define bsDMA_SRC_BURST_SIZE     24

#define DMA_RX_CHANNEL_ID  1
#define DMA_TX_CHANNEL_ID  0

DMA_Descriptor test __attribute__((aligned (8)));
char src[] = "Finished to receive a frame!\n";

static char dst[256];
static int index_uart = 0;


static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
  
    NVIC_InitStructure.NVIC_IRQChannel = UART0_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = 1;
    /* init NVIC */
    NVIC_Init(&NVIC_InitStructure);
}

void config_uart(uint32_t freq, uint32_t baud)
{
    UART_sStateStruct config;

    config.word_length       = UART_WLEN_8_BITS;
    config.parity            = UART_PARITY_NOT_CHECK;
    config.fifo_enable       = 1;
    config.two_stop_bits     = 0;
    config.receive_en        = 1;
    config.transmit_en       = 1;
    config.UART_en           = 1;
    config.cts_en            = 0;
    config.rts_en            = 0;
    config.rxfifo_waterlevel = 1;
    config.txfifo_waterlevel = 1;
    config.ClockFrequency    = freq;
    config.BaudRate          = baud;

    apUART_Initialize(APB_UART0, &config, UART_INTBIT_RECEIVE | UART_INTBIT_TRANSMIT);
}

void apUART_SET_INT_FIFO(UART_TypeDef* pBase)
{
    pBase->FifoSelect = 0x0100;
    pBase->IntMask |= 1<<bsUART_TIMEOUT_INTENAB;
}



//test function about uart sending
void uart_send_test(const char *data, int len)
{
    for (int i = 0; i < len; i++)
    {
        while(!apUART_Check_TXFIFO_EMPTY(APB_UART0))
        {
            ;
        }
            UART_SendData(APB_UART0,data[i]);
    }
}

uint8_t uart_read_test(void)
{
    while(apUART_Check_RXFIFO_EMPTY(APB_UART0))
    {
        ;
    }
    return UART_ReceData(APB_UART0);
}

void all_send_test_data(void)
{
    uint8_t data = 0x15;
    while(1)
    {
        uart_send_test((const char*)&data,1);
        SYSCTRL_DelayCycles(24000000,100);
    }
}

void uart_fifo_recive_init(void)
{
    SYSCTRL_ClearClkGate(SYSCTRL_ClkGate_APB_GPIO0);//gpio0
    SYSCTRL_ClearClkGateMulti((1 << SYSCTRL_ClkGate_APB_UART0)
                          | (1 << SYSCTRL_ClkGate_APB_PinCtrl));
    PINCTRL_SetPadMux(GIO_GPIO_1, IO_SOURCE_UART0_TXD);
    PINCTRL_SelUartRxdIn(UART_PORT_1,GIO_GPIO_2);
    config_uart(24000000,115200);
    
    APB_UART0->FifoSelect = (0 << bsUART_TRANS_INT_LEVEL ) |
                            (0X18 << bsUART_RECV_INT_LEVEL  ) ;
    APB_UART0->IntMask = (1 << bsUART_RECEIVE_INTENAB) | (0 << bsUART_TRANSMIT_INTENAB) |
                         (1 << bsUART_TIMEOUT_INTENAB);
    APB_UART0->Control = 1 << bsUART_RECEIVE_ENABLE |
                         1 << bsUART_TRANSMIT_ENABLE |
                         1 << bsUART_ENABLE          |
                         0 << bsUART_CTS_ENA         |
                         0 << bsUART_RTS_ENA;

}

void dma_uart_init()
{
    DEBUG_LOG("setup_peripheral_dma\n");
    SYSCTRL_ClearClkGate(SYSCTRL_ITEM_APB_DMA);

    //配置DMA接收
    APB_DMA->Channels[1].Descriptor.Ctrl = ((uint32_t)0x0 << bsDMA_INT_C_MASK)
                                           | ((uint32_t)0x0 <<bsDMA_DST_REQ_SEL)
                                           | ((uint32_t)0x0 << bsDMA_SRC_REQ_SEL)
                                           | ((uint32_t)0x0 << bsDMA_DST_ADDR_CTRL)
                                           | ((uint32_t)0x2 << bsDMA_SRC_ADDR_CTRL)   //DMA_ADDRESS_FIXED
                                           | ((uint32_t)0x0 << bsDMA_DST_MODE)
                                           | ((uint32_t)0x1 << bsDMA_SRC_MODE)     //
                                           | ((uint32_t)0x0 << bsDMA_DST_WIDTH)
                                           | ((uint32_t)0x0 << bsDMA_SRC_WIDTH)
                                           | ((uint32_t)0x2 << bsDMA_SRC_BURST_SIZE); //4 transefers

    APB_DMA->Channels[1].Descriptor.SrcAddr = (uint32_t)&APB_UART0->DataRead;
    APB_DMA->Channels[1].Descriptor.DstAddr = (uint32_t)dst;
    APB_DMA->Channels[1].Descriptor.TranSize = 48;

    DMA_EnableChannel(1, &APB_DMA->Channels[1].Descriptor);
}

//添加UART通过DMA发送的配置
void UART_trigger_DmaSend(void)
{
    DMA_PrepareMem2Peripheral(  &test,
                                SYSCTRL_DMA_UART0_TX,
                                (uint32_t*)src, strlen(src),
                                DMA_ADDRESS_INC, 0);
    DMA_EnableChannel(DMA_TX_CHANNEL_ID, &test);
}

void uart_fifo_dma_int() {
    SYSCTRL_ClearClkGate(SYSCTRL_ClkGate_APB_GPIO0);//gpio0
    SYSCTRL_ClearClkGateMulti((1 << SYSCTRL_ClkGate_APB_UART0)
                              | (1 << SYSCTRL_ClkGate_APB_PinCtrl));
    PINCTRL_SetPadMux(GIO_GPIO_1, IO_SOURCE_UART0_TXD);
    PINCTRL_SelUartRxdIn(UART_PORT_1, GIO_GPIO_2);
    config_uart(24000000, 115200);

    APB_UART0->FifoSelect = (0 << bsUART_TRANS_INT_LEVEL) |
                            (0X7 << bsUART_RECV_INT_LEVEL);
    APB_UART0->IntMask = (0 << bsUART_RECEIVE_INTENAB) | (0 << bsUART_TRANSMIT_INTENAB) |
                         (1 << bsUART_TIMEOUT_INTENAB);
    APB_UART0->Control = 1 << bsUART_RECEIVE_ENABLE |
                         1 << bsUART_TRANSMIT_ENABLE |
                         1 << bsUART_ENABLE |
                         0 << bsUART_CTS_ENA |
                         0 << bsUART_RTS_ENA;
}

uint8_t rc_data = 0;

static void uart_int_status_callback(void)
{
    if(apUART_Get_ITStatus(APB_UART0,bsUART_TRANSMIT_INTENAB))
    {
        DEBUG_LOG("UART0_IRQHandler transferred\r\n");
        UART_SendData(APB_UART0,0x52);
        apUART_Clr_TX_INT(APB_UART0);
        apUART_Disable_TRANSMIT_INT(APB_UART0);
    }
    if(apUART_Get_ITStatus(APB_UART0,bsUART_RECEIVE_INTENAB))
    {
        DEBUG_LOG("received data\r\n");
        rc_data = UART_ReceData(APB_UART0);
        UART_SendData(APB_UART0,rc_data);
        apUART_Clr_RECEIVE_INT(APB_UART0);
    }
    if(apUART_Get_ITStatus(APB_UART0,bsUART_TIMEOUT_INTENAB))
    {
        DEBUG_LOG("received data timeout\r\n");
        APB_UART0->IntClear |= (1<<bsUART_TIMEOUT_INTENAB);
    }
}

static void uart_int_fifo_callback(void)
{
    uint32_t status;

    while(1)
    {
        status = apUART_Get_all_raw_int_stat(APB_UART0);

        if (status == 0)
            break;

        APB_UART0->IntClear = status;

        // rx int
        if (status & (1 << bsUART_RECEIVE_INTENAB))
        {
            while (apUART_Check_RXFIFO_EMPTY(APB_UART0) != 1)
            {
                char c = APB_UART0->DataRead;
                dst[index_uart] = c;
                index_uart++;
            }
            DEBUG_LOG("full init\r\n");
        }

        // rx timeout_int
        if (status & (1 << bsUART_TIMEOUT_INTENAB))
        {
            while (apUART_Check_RXFIFO_EMPTY(APB_UART0) != 1)
            {
                char c = APB_UART0->DataRead;
                dst[index_uart] = c;
                index_uart++;
            }
            DEBUG_LOG("time out");
        }
        dst[index_uart] = 0;
        index_uart = 0;
        DEBUG_LOG("\ndst = %s\n",dst);
    }
}

static void uart_int_dma_callback(void)
{
    uint32_t status;
    DEBUG_LOG("@%x #%x #%x\n",APB_UART0->IntMask,APB_UART0->IntRaw,APB_UART0->Interrupt);

    while(1)
    {
        status = apUART_Get_all_raw_int_stat(APB_UART0);

        if (status == 0)
            break;

        APB_UART0->IntClear = status;

        // rx timeout_int
        if (status & (1 << bsUART_TIMEOUT_INTENAB))
        {
            while (apUART_Check_RXFIFO_EMPTY(APB_UART0) != 1)
            {
                char c = APB_UART0->DataRead;
                int index_uart = APB_DMA->Channels[1].Descriptor.DstAddr - (uint32_t)dst;
                dst[index_uart] = c;
                if (index_uart == 255)
                {
                    APB_DMA->Channels[1].Descriptor.DstAddr = (uint32_t)dst;
                    UART_trigger_DmaSend();
                }
                else
                    APB_DMA->Channels[1].Descriptor.DstAddr++;

                APB_DMA->Channels[1].Descriptor.TranSize = 48;
            }
            DEBUG_LOG("\nlen=%d, dst = %s\n",APB_DMA->Channels[1].Descriptor.TranSize,dst);
        }
    }
}
void UART0_IRQHandler(void)
{
//    uart_int_status_callback();
    uart_int_fifo_callback();
//    uart_int_dma_callback();
}

void uart_test_init(void)
{
    NVIC_Configuration();
    uart_fifo_recive_init();
}

void uart_test_fifo_recive(void)
{
    uart_test_init();
//    all_send_test_data();

    while(1)
    {
        ;
    }
}

void uart_test_dma_recive(void)
{
    NVIC_Configuration();
    uart_fifo_dma_int();
    dma_uart_init();
    
    while (1)
    {
        UART_trigger_DmaSend();
        SYSCTRL_DelayCycles(1000,1000);
    }
}


