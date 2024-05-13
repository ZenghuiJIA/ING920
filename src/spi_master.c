#include "spi_master.h"

#define SPI_MIC_CLK         GIO_GPIO_9
#define SPI_MIC_MOSI        GIO_GPIO_10
#define SPI_MIC_MISO        GIO_GPIO_11
#define SPI_MIC_CS          GIO_GPIO_12
//如果使用 QSPI 则使用如下两个引脚SPI不使用
#define SPI_MIC_WP          GIO_GPIO_13
#define SPI_MIC_HOLD        GIO_GPIO_14

static void spi_pin_init(void) {
    SYSCTRL_ClearClkGateMulti((1 << SYSCTRL_ITEM_APB_SPI1)
                              | (1 << SYSCTRL_ITEM_APB_PinCtrl));

//    PINCTRL_SelSpiIn(SPI_PORT_1, SPI_MIC_CLK, SPI_MIC_CS, SPI_MIC_HOLD,
//                     SPI_MIC_WP, SPI_MIC_MISO, SPI_MIC_MOSI);
    PINCTRL_SetPadMux(SPI_MIC_CLK, IO_SOURCE_SPI1_CLK_OUT);
    PINCTRL_SetPadMux(SPI_MIC_CS, IO_SOURCE_SPI1_CSN_OUT);
    PINCTRL_SetPadMux(SPI_MIC_MOSI, IO_SOURCE_SPI1_MOSI_OUT);
}

#define SPI_MASTER_PARAM(DataLen) { SPI_INTERFACETIMINGSCLKDIV_DEFAULT_2M, \
SPI_CPHA_ODD_SCLK_EDGES, SPI_CPOL_SCLK_LOW_IN_IDLE_STATES, \
SPI_LSB_MOST_SIGNIFICANT_BIT_FIRST, SPI_DATALEN_8_BITS, \
SPI_SLVMODE_MASTER_MODE, SPI_TRANSMODE_WRITE_READ, \
SPI_DUALQUAD_REGULAR_MODE, DataLen, DataLen, \
SPI_ADDREN_DISABLE, SPI_CMDEN_DISABLE, (1 << bsSPI_INTREN_ENDINTEN), \
0, 0, SPI_SLVDATAONLY_ENABLE, SPI_ADDRLEN_1_BYTE }

#define DATA_LEN (SPI_FIFO_DEPTH)
static void spi_peripheral_init(void)
{
    apSSP_sDeviceControlBlock pParam = SPI_MASTER_PARAM(DATA_LEN);
    apSSP_DeviceParametersSet(APB_SSP1, &pParam);
}

static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = APBSPI_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = 1;
    /* init NVIC */
    NVIC_Init(&NVIC_InitStructure);
}


void APBSPI_IRQHandler(void)
{
    uint32_t stat = apSSP_GetIntRawStatus(APB_SSP1);

    if(stat & (1 << bsSPI_INTREN_ENDINTEN))
    {
        apSSP_ClearIntStatus(APB_SSP1, 1 << bsSPI_INTREN_ENDINTEN);
    }
}

uint32_t write_data[DATA_LEN] = {0,1,2,3};
void spi_send_data(void)
{
    uint8_t  i;
    apSSP_WriteCmd(APB_SSP1, 0x00, 0x00);//trigger transfer
//    for(i = 0; i < DATA_LEN; i++)
//    {
//        if(apSSP_TxFifoFull(APB_SSP1)){ break; }
//        apSSP_WriteFIFO(APB_SSP1, write_data[i]);
//    }
        apSSP_WriteFIFO(APB_SSP1, 0x05);
    while(apSSP_GetSPIActiveStatus(APB_SSP1));
}

uint32_t read_data[DATA_LEN];
void spi_read_data(void)
{
    uint8_t i;
    uint32_t num = apSSP_GetDataNumInRxFifo(APB_SSP1);
    for(i = 0; i < num; i++)
    {
        apSSP_ReadFIFO(APB_SSP1, &read_data[i]);
    }
}

void spi_init(void)
{
    spi_pin_init();
    spi_peripheral_init();
    NVIC_Configuration();
}

void spi_test(void)
{
    spi_init();
//    spi_send_data();
    while(1)
    {
        spi_send_data();
        SYSCTRL_DelayCycles(1000,200);
    }
}

