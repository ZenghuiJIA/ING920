#include "spi_master.h"
#include "misc.h"
#define SPI_MIC_CLK         GIO_GPIO_9

#define SPI_MIC_MOSI        GIO_GPIO_10
#define SPI_MIC_MISO        GIO_GPIO_11
#define SPI_MIC_CS          GIO_GPIO_12
//如果使用 QSPI 则使用如下两个引脚SPI不使用
#define SPI_MIC_WP          GIO_GPIO_15
#define SPI_MIC_HOLD        GIO_GPIO_14

#define DATA_LEN (SPI_FIFO_DEPTH)

//#define SPI_MODE            APB_SSP1
//#define SPI_IRQ             APBSPI_IRQ
//#define SPI_IRQ_HANDER      APBSPI_IRQHandler
//#define SPI_CLK             SYSCTRL_ITEM_APB_SPI1
//#define SPI_PORT            SPI_PORT_1

#define SPI_MODE            AHB_SSP0
#define SPI_IRQ             QSPI_IRQ
#define SPI_IRQ_HANDER      QSPI_IRQHandler
#define SPI_CLK             SYSCTRL_ITEM_AHB_SPI0
#define SPI_PORT            SPI_PORT_0
static void setup_peripherals_spi_module(void)
{
    apSSP_sDeviceControlBlock pParam;

    pParam.eSclkDiv = SPI_INTERFACETIMINGSCLKDIV_DEFAULT_2M;
    pParam.eSCLKPolarity = SPI_CPOL_SCLK_LOW_IN_IDLE_STATES;
    pParam.eSCLKPhase = SPI_CPHA_ODD_SCLK_EDGES;
    pParam.eLsbMsbOrder = SPI_LSB_MOST_SIGNIFICANT_BIT_FIRST;
    pParam.eDataSize = SPI_DATALEN_8_BITS;
    pParam.eMasterSlaveMode = SPI_SLVMODE_MASTER_MODE;
    pParam.eReadWriteMode = SPI_TRANSMODE_WRITE_READ;
    pParam.eQuadMode = SPI_DUALQUAD_REGULAR_MODE;
    pParam.eWriteTransCnt = DATA_LEN;
    pParam.eReadTransCnt = DATA_LEN;
    pParam.eAddrEn = SPI_ADDREN_DISABLE;
    pParam.eCmdEn = SPI_CMDEN_ENABLE;
    pParam.RxThres = DATA_LEN/2;
    pParam.TxThres = DATA_LEN/2;
    pParam.SlaveDataOnly = SPI_SLVDATAONLY_ENABLE;
    pParam.eAddrLen = SPI_ADDRLEN_1_BYTE;
//    pParam.eInterruptMask = (1 << bsSPI_INTREN_ENDINTEN);
    pParam.eMOSI_Dir = SPI_MOSI_BI_DIR_MODE;

    apSSP_DeviceParametersSet(SPI_MODE, &pParam);
}

static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
  
    NVIC_InitStructure.NVIC_IRQChannel = SPI_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = 1;
    /* init NVIC */
    NVIC_Init(&NVIC_InitStructure);
}

static uint32_t peripherals_spi_isr(void *user_data)
{
    uint32_t stat = apSSP_GetIntRawStatus(SPI_MODE);

    if(stat & (1 << bsSPI_INTREN_ENDINTEN))
    {
        apSSP_ClearIntStatus(SPI_MODE, 1 << bsSPI_INTREN_ENDINTEN);
    }
    return 0;
}

void SPI_IRQ_HANDER(void)
{
    peripherals_spi_isr(NULL);
}

static void setup_peripherals_spi_pin(void)
{
    SYSCTRL_ClearClkGateMulti(    (1 << SPI_CLK)
                                  | (1 << SYSCTRL_ITEM_APB_PinCtrl));

    PINCTRL_SelSpiIn(SPI_PORT_1, SPI_MIC_CLK, SPI_MIC_CS, IO_NOT_A_PIN,
                     IO_NOT_A_PIN, IO_NOT_A_PIN, SPI_MIC_MOSI);
    PINCTRL_SetPadMux(SPI_MIC_CLK, IO_SOURCE_SPI1_CLK_OUT);
    PINCTRL_SetPadMux(SPI_MIC_CS, IO_SOURCE_SPI1_CSN_OUT);
    PINCTRL_SetPadMux(SPI_MIC_MOSI, IO_SOURCE_SPI1_MOSI_OUT);

}
uint32_t write_data[DATA_LEN];
//void peripherals_spi_send_data(void)
//{
//    GIO_WriteValue(SPI_MIC_CS, 0);
//    uint8_t i;
//  apSSP_WriteCmd(SPI_MODE, 0x00, 0x00);//trigger transfer
//  for(i = 0; i < DATA_LEN; i++)
//  {
//    if(apSSP_TxFifoFull(SPI_MODE)){ break; }
//    apSSP_WriteFIFO(SPI_MODE, 0x05);
//  }
//  while(!apSSP_TxFifoEmpty(SPI_MODE));
//
////  while(apSSP_GetSPIActiveStatus(SPI_MODE));
//  GIO_WriteValue(SPI_MIC_CS, 1);
//}

void write_byte(uint32_t data)
{
//    while(!apSSP_TxFifoEmpty(SPI_MODE));
    apSSP_WriteFIFO(SPI_MODE, data);
}

uint32_t read_buf[20];
void read_data(void)
{
//    GIO_WriteValue(SPI_MIC_CS, 0);
    apSSP_SetTransferControlWrTranCnt(SPI_MODE,3);
    apSSP_SetTransferControlRdTranCnt(SPI_MODE,8);
//    apSSP_ResetTxFifo(SPI_MODE);
    apSSP_WriteCmd(SPI_MODE, 0x00, 0x03);//trigger transfer

//    write_byte(0x05);
    write_byte(0x01);
    write_byte(0xF0);
    write_byte(0x11);
//    write_byte(0x22);
//    write_byte(0x33);
//    write_byte(0x44);
//    write_byte(0x55);
//    write_byte(0x55);
//    while(apSSP_GetSPIActiveStatus(AHB_SSP0));
//    uint32_t num = apSSP_GetDataNumInRxFifo(AHB_SSP0);
    apSSP_ResetRxFifo(SPI_MODE);
    for(uint8_t i = 0; i < 8; i++)
    {
        apSSP_ReadFIFO(SPI_MODE, &read_buf[i]);
    }
//    for(uint8_t i = 0; i < DATA_LEN;i++)
//    {
//        write_byte(0x05);
////        if(apSSP_TxFifoFull(SPI_MODE)){ break; }
////        apSSP_WriteFIFO(SPI_MODE, read_buf[i]);
//    }
//    GIO_WriteValue(SPI_MIC_CS, 1);
    while(apSSP_GetSPIActiveStatus(SPI_MODE));
}


void peripherals_spi_send_data(void)
{
    uint32_t i;
    static uint32_t data = 0x021111;

    write_data[0] = ++data;
    apSSP_WriteCmd(SPI_MODE, 0x00, 0x00);//trigger transfer
    for(i = 0; i < DATA_LEN; i++)
    {
        apSSP_WriteFIFO(SPI_MODE, write_data[i]);
    }
    while(apSSP_GetSPIActiveStatus(SPI_MODE));
    uint32_t num = apSSP_GetDataNumInRxFifo(SPI_MODE);
    for(i = 0; i < num; i++)
    {
        apSSP_ReadFIFO(SPI_MODE, &read_buf[i]);
    }
}

void spi_test(void)
{
    setup_peripherals_spi_pin();
    setup_peripherals_spi_module();
    NVIC_Configuration();
    read_data();
//    peripherals_spi_send_data();
    while(1)
    {
//        peripherals_spi_send_data();
        SYSCTRL_DelayCycles(24000,1000);
        read_data();
    }
}

