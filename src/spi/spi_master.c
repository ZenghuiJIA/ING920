#include "spi_master.h"
#include "ingsoc.h"
#include "misc.h"


#if 1
#include "SEGGER_RTT.h"
#include "debug_print.h"
#define DEBUG_LOG(...)      SEGGER_RTT_printf(0,##__VA_ARGS__)

#define DEBUG_UART_log      debug_uart
#else
#define DEBUG_LOG(...)
#define DEBUG_UART_log(...)
#endif

/*
 * 注意spi进行数据通信时，模式一定要匹配，从机数据发送和接收都需要主机提供clk。
 * 关于 SPI 头文件相关参数的使用说明
 *   SPI_TRANSMODE_WRITE_READ_SAME_TIME = 0, 数据同时读写
  SPI_TRANSMODE_WRITE_ONLY = 1, 仅写数据
  SPI_TRANSMODE_READ_ONLY = 2, 仅读数据
  SPI_TRANSMODE_WRITE_READ = 3,先读后写
  SPI_TRANSMODE_READ_WRITE = 4,先写后读
  SPI_TRANSMODE_WRITE_DUMMY_READ = 5, 先写后读在读写之间添加 dummy 默认8个时钟周期
  SPI_TRANSMODE_READ_DUMMY_WRITE = 6, 先读后写在读写之间添加 dummy 默认8个时钟周期
  SPI_TRANSMODE_NONE_DATA = 7, 空数据只发送address和commod
  SPI_TRANSMODE_DUMMY_WRITE = 8, 在读之前添加 dummy 默认8个时钟周期
  SPI_TRANSMODE_DUMMY_READ = 9 在写之前添加 dummy 默认8个时钟周期

  eMOSI_Dir = SPI_MOSI_UNI_DIR_MODE; IO模式为单向模式
  SPI_MOSI_BI_DIR_MODE 双向模式

  SPI_DUALQUAD_REGULAR_MODE = 0,//全双工模式，双线数据全双工
  SPI_DUALQUAD_DUAL_IO_MODE = 1,//半双工模式，双线数据双向传输 （该模式仅对于SPI FLASH有效）
  SPI_DUALQUAD_QUAD_IO_MODE = 2 //四线半双工模式，四线数据双向传输


  spi进行数据传输接收和写入数据时，单次写入或者读出数据的长度必须小于256字节，否则需要特殊处理
  这里仅演示小于256字节读写的形式，flash 1page
 *
 */

#define MASTER
// #define SLAVE
#define SPI_MIC_CLK         GIO_GPIO_9

#define SPI_MIC_MOSI        GIO_GPIO_10
#define SPI_MIC_MISO        GIO_GPIO_11
#define SPI_MIC_CS          GIO_GPIO_12
//如果使用 QSPI 则使用如下两个引脚SPI不使用
#define SPI_MIC_WP          GIO_GPIO_15
#define SPI_MIC_HOLD        GIO_GPIO_16

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

    //速率选择
    pParam.eSclkDiv = SPI_INTERFACETIMINGSCLKDIV_DEFAULT_2M;
    pParam.eSCLKPolarity = SPI_CPOL_SCLK_LOW_IN_IDLE_STATES;
    pParam.eSCLKPhase = SPI_CPHA_ODD_SCLK_EDGES;
    pParam.eLsbMsbOrder = SPI_LSB_MOST_SIGNIFICANT_BIT_FIRST;
    pParam.eDataSize = SPI_DATALEN_8_BITS;
    #ifdef MASTER
    pParam.eMasterSlaveMode = SPI_SLVMODE_MASTER_MODE;//主从模式
    #else
    pParam.eMasterSlaveMode = SPI_SLVMODE_SLAVE_MODE;
    #endif
    pParam.eReadWriteMode = SPI_TRANSMODE_WRITE_READ;
    pParam.eQuadMode = SPI_DUALQUAD_REGULAR_MODE;//设置spi传输使用的io模式两线三线四线
    //单次发送数据大小 fifo满
    pParam.eWriteTransCnt = DATA_LEN;
    pParam.eReadTransCnt = DATA_LEN;
    pParam.eAddrEn = SPI_ADDREN_DISABLE;//地址使能
    pParam.eCmdEn = SPI_CMDEN_DISABLE;//命令使能
    //中断触发fifo阈值 半满中断
    pParam.RxThres = DATA_LEN/2;
    pParam.TxThres = DATA_LEN/2;
    //仅数据模式，该模式仅从机模式有效，必须处于全双工模式
    pParam.SlaveDataOnly = SPI_SLVDATAONLY_ENABLE;
    //地址长度 flash型号使用3字节地址
    pParam.eAddrLen = SPI_ADDRLEN_3_BYTES;
    //中断触发掩码
    pParam.eInterruptMask = (1 << bsSPI_INTREN_ENDINTEN)|(1<<bsSPI_INTREN_RXFIFOINTEN);
    pParam.eMOSI_Dir = SPI_MOSI_UNI_DIR_MODE;

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
    uint32_t data = 0,i;
    uint32_t stat = apSSP_GetIntRawStatus(SPI_MODE);

    if(stat & (1 << bsSPI_INTREN_ENDINTEN))
    {
        DEBUG_LOG(">>>spi end\r\n");
        uint32_t num = apSSP_GetDataNumInRxFifo(APB_SSP1);
        for(i = 0; i < num; i++)
        {
            apSSP_ReadFIFO(APB_SSP1, &data);
            DEBUG_LOG(">>>spi rx data:0x%x\r\n",data);
        }
        
    }
    if(stat & (1<<bsSPI_INTREN_RXFIFOINTEN))
    {
        while(apSSP_RxFifoEmpty(SPI_MODE) == 0)
        {
        apSSP_ReadFIFO(SPI_MODE, &data);
        DEBUG_LOG(">>>spi rx data:0x%x\r\n",data);
        }
    }
    apSSP_ClearIntStatus(SPI_MODE, 0xff);
    return 0;
}

void SPI_IRQ_HANDER(void)
{
    peripherals_spi_isr(0);
}

static void setup_peripherals_spi_pin(void)
{
    SYSCTRL_ClearClkGateMulti(    (1 << SPI_CLK)
    |(1<<SYSCTRL_ITEM_APB_SPI1)
                                  | (1 << SYSCTRL_ITEM_APB_PinCtrl)
                                  | (1 << SYSCTRL_ITEM_APB_GPIO0));
    //该函数可直接选择iomox输入和输出双向配置
    PINCTRL_SelSpiIn(SPI_PORT, SPI_MIC_CLK, SPI_MIC_CS, SPI_MIC_HOLD,
                     SPI_MIC_WP, SPI_MIC_MISO, SPI_MIC_MOSI);
    PINCTRL_Pull(SPI_MIC_CS,PINCTRL_PULL_UP);

}

/*
 * 测试spi master 发送数据的几个测试实例
   1. 发送8个字节数据只写数据
   2. 发送8个字节数据同时读写
   3. 读取8个字节数据只读数据
   4. 先写后读写4读7
   5. 先读后写
   6. dma数据传输
 */
uint8_t write_data[8] = {0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};
void spi_test(void)
{
    uint32_t i,timeout;
    setup_peripherals_spi_pin();
    setup_peripherals_spi_module();
    NVIC_Configuration();
    DEBUG_LOG("start spi test\r\n");
    DEBUG_LOG("send 8 bytes data only write\r\n");
    apSSP_SetAddrEn(SPI_MODE, 0);
    apSSP_SetCmdEn(SPI_MODE, 0);
    apSSP_SetTransMode(SPI_MODE,SPI_TRANSMODE_WRITE_ONLY);
    apSSP_SetTransferControlWrTranCnt(SPI_MODE,8);
    apSSP_SetTransferControlRdTranCnt(SPI_MODE,0);
    apSSP_WriteCmd(SPI_MODE, 0, 0);
    for(i=0;i<8;i++)
    {
        apSSP_WriteFIFO(SPI_MODE, write_data[i]);
    }
    
    
    while (apSSP_GetSPIActiveStatus(SPI_MODE));
    DEBUG_LOG("send 8 bytes data write read\r\n");
    apSSP_SetTransMode(SPI_MODE,SPI_TRANSMODE_WRITE_READ_SAME_TIME);
    apSSP_SetTransferControlWrTranCnt(SPI_MODE,8);
    apSSP_SetTransferControlRdTranCnt(SPI_MODE,8);
    apSSP_WriteCmd(SPI_MODE, 0, 0);
    for(i=0;i<8;i++)
    {
        apSSP_WriteFIFO(SPI_MODE, write_data[i]);
    }
    
    while (apSSP_GetSPIActiveStatus(SPI_MODE));
    
    DEBUG_LOG("read 8 bytes data only read\r\n");
    apSSP_SetTransMode(SPI_MODE,SPI_TRANSMODE_READ_ONLY);
    apSSP_SetTransferControlWrTranCnt(SPI_MODE,0);
    apSSP_SetTransferControlRdTranCnt(SPI_MODE,8);
    apSSP_WriteCmd(SPI_MODE, 0, 0);
    while (apSSP_GetSPIActiveStatus(SPI_MODE));

    DEBUG_LOG("data write read\r\n");
    apSSP_SetTransMode(SPI_MODE,SPI_TRANSMODE_WRITE_READ);
    apSSP_SetTransferControlWrTranCnt(SPI_MODE,4);
    apSSP_SetTransferControlRdTranCnt(SPI_MODE,6);
    apSSP_WriteCmd(SPI_MODE, 0, 0);
    for(i=0;i<4;i++)
    {
        apSSP_WriteFIFO(SPI_MODE, write_data[i]);
    }
    
    while (apSSP_GetSPIActiveStatus(SPI_MODE));

    DEBUG_LOG("read wite\r\n");
    apSSP_SetTransMode(SPI_MODE,SPI_TRANSMODE_READ_WRITE);
    apSSP_SetTransferControlWrTranCnt(SPI_MODE,4);
    apSSP_SetTransferControlRdTranCnt(SPI_MODE,6);
    apSSP_WriteCmd(SPI_MODE, 0, 0);
    for(i=0;i<4;i++)
    {
        apSSP_WriteFIFO(SPI_MODE, write_data[i]);
    }
    
    while (apSSP_GetSPIActiveStatus(SPI_MODE));
    DEBUG_LOG("end test\r\n");

    while (1)
    {
        /* code */
    }
    

}

