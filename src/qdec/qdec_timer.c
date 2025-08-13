#include "qdec_timer.h"
#include "ingsoc.h"
#include <stdio.h>
#include "misc.h"

#if 1
#include "SEGGER_RTT.h"
#define DEBUG_LOG(...)      SEGGER_RTT_printf(0,##__VA_ARGS__)
#else
#define DEBUG_LOG(...)
#define DEBUG_UART_log(...)
#endif

#define QDEC_CHX    QDEC_CH0

static void NVIC_ConfigurationQdec0(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
  
    NVIC_InitStructure.NVIC_IRQChannel = QDEC0_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = 1;
    /* init NVIC */
    NVIC_Init(&NVIC_InitStructure);
}
static void NVIC_ConfigurationQdec1(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
  
    NVIC_InitStructure.NVIC_IRQChannel = QDEC1_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = 1;
    /* init NVIC */
    NVIC_Init(&NVIC_InitStructure);
}
static void NVIC_ConfigurationQdec2(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
  
    NVIC_InitStructure.NVIC_IRQChannel = QDEC2_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = 1;
    /* init NVIC */
    NVIC_Init(&NVIC_InitStructure);
}

uint32_t  qdec2_callback(void *user_data)
{
    uint32_t state,pcap; 
    state = QDEC_GetChxIntStateClr(QDEC_CHX);
//    pcap = *(uint32_t*)(0x40009018ul);
    pcap = QDEC_GetChxReadRab(QDEC_CHX);
    printf("state:0x%x\r\n",state);
    printf("pcap vale:0x%x\r\n",(uint16_t)pcap);
    pcap = *(uint32_t*)(0x40009010ul);
    printf("pcap vale:0x%x\r\n",(uint16_t)pcap);
    if(state&QDEC_INT_BUF_FULL_STATE)
    {
        printf("QDEC_INT_BUF_FULL_STATE\r\n");
    }
    if(state&QDEC_INT_PDC_END_STATE)
    {
        printf("QDEC_INT_PDC_END_STATE\r\n");
    }
    if(state&QDEF_INT_ETRGS_CMB_STATE)
    {
        printf("QDEF_INT_ETRGS_CMB_STATE\r\n");
    }
    if(state&QDEC_INT_LDRBS_CMB_STATE)
    {
        printf("QDEC_INT_LDRBS_CMB_STATE\r\n");
    }
    if(state&QDEC_INT_LDRAS_CMB_STATE)
    {
        printf("QDEC_INT_LDRAS_CMB_STATE\r\n");
    }
    if(state&QDEC_INT_CPCS_CMB_STATE)
    {
        
        printf("QDEC_INT_CPCS_CMB_STATE\r\n");
    }
    if(state&QDEC_INT_CPBS_CMB_STATE)
    {
        printf("QDEC_INT_CPBS_CMB_STATE\r\n");
    }
    if(state&QDEC_INT_CPAS_CMB_STATE)
    {
        printf("QDEC_INT_CPAS_CMB_STATE\r\n");
    }
    if(state&QDEC_INT_LOVFS_CMB_STATE)
    {
        printf("QDEC_INT_LOVFS_CMB_STATE\r\n");
    }
    if(state&QDEC_INT_COVFS_CMB_STATE)
    {
        printf("QDEC_INT_COVFS_CMB_STATE\r\n");
    }
    return 0;
}

uint32_t  qdec1_callback(void *user_data)
{
    uint32_t state,pcap; 
    state = QDEC_GetChxIntStateClr(QDEC_CHX);
//    pcap = *(uint32_t*)(0x40009018ul);
    pcap = QDEC_GetChxReadRab(QDEC_CHX);
    printf("state:0x%x\r\n",state);
    printf("pcap vale:0x%x\r\n",(uint16_t)pcap);
    pcap = *(uint32_t*)(0x40009050ul);
    printf("pcap vale:0x%x\r\n",(uint16_t)pcap);
    if(state&QDEC_INT_BUF_FULL_STATE)
    {
        printf("QDEC_INT_BUF_FULL_STATE\r\n");
    }
    if(state&QDEC_INT_PDC_END_STATE)
    {
        printf("QDEC_INT_PDC_END_STATE\r\n");
    }
    if(state&QDEF_INT_ETRGS_CMB_STATE)
    {
        printf("QDEF_INT_ETRGS_CMB_STATE\r\n");
    }
    if(state&QDEC_INT_LDRBS_CMB_STATE)
    {
        printf("QDEC_INT_LDRBS_CMB_STATE\r\n");
    }
    if(state&QDEC_INT_LDRAS_CMB_STATE)
    {
        printf("QDEC_INT_LDRAS_CMB_STATE\r\n");
    }
    if(state&QDEC_INT_CPCS_CMB_STATE)
    {
        printf("QDEC_INT_CPCS_CMB_STATE\r\n");
    }
    if(state&QDEC_INT_CPBS_CMB_STATE)
    {
        printf("QDEC_INT_CPBS_CMB_STATE\r\n");
    }
    if(state&QDEC_INT_CPAS_CMB_STATE)
    {
        printf("QDEC_INT_CPAS_CMB_STATE\r\n");
    }
    if(state&QDEC_INT_LOVFS_CMB_STATE)
    {
        printf("QDEC_INT_LOVFS_CMB_STATE\r\n");
    }
    if(state&QDEC_INT_COVFS_CMB_STATE)
    {
        printf("QDEC_INT_COVFS_CMB_STATE\r\n");
    }
    return 0;
}

uint32_t  qdec0_callback(void *user_data)
{
    uint32_t state,pcap; 
    state = QDEC_GetChxIntStateClr(QDEC_CHX);
//    pcap = *(uint32_t*)(0x40009018ul);
    pcap = QDEC_GetChxReadRab(QDEC_CHX);
    printf("state:0x%x\r\n",state);
    printf("pcap vale:0x%x\r\n",(uint16_t)pcap);
    pcap = *(uint32_t*)(0x40009010ul);
    printf("pcap vale:0x%x\r\n",(uint16_t)pcap);
    if(state&QDEC_INT_BUF_FULL_STATE)
    {
        printf("QDEC_INT_BUF_FULL_STATE\r\n");
    }
    if(state&QDEC_INT_PDC_END_STATE)
    {
        printf("QDEC_INT_PDC_END_STATE\r\n");
    }
    if(state&QDEF_INT_ETRGS_CMB_STATE)
    {
        printf("QDEF_INT_ETRGS_CMB_STATE\r\n");
    }
    if(state&QDEC_INT_LDRBS_CMB_STATE)
    {
        printf("QDEC_INT_LDRBS_CMB_STATE\r\n");
    }
    if(state&QDEC_INT_LDRAS_CMB_STATE)
    {
        printf("QDEC_INT_LDRAS_CMB_STATE\r\n");
    }
    if(state&QDEC_INT_CPCS_CMB_STATE)
    {
        printf("QDEC_INT_CPCS_CMB_STATE\r\n");
//        extern void QdecStart(void);
//        QdecStart();
    }
    if(state&QDEC_INT_CPBS_CMB_STATE)
    {
        printf("QDEC_INT_CPBS_CMB_STATE\r\n");
    }
    if(state&QDEC_INT_CPAS_CMB_STATE)
    {
        printf("QDEC_INT_CPAS_CMB_STATE\r\n");
    }
    if(state&QDEC_INT_LOVFS_CMB_STATE)
    {
        printf("QDEC_INT_LOVFS_CMB_STATE\r\n");
    }
    if(state&QDEC_INT_COVFS_CMB_STATE)
    {
        printf("QDEC_INT_COVFS_CMB_STATE\r\n");
    }
    return 0;
}

//static void QDEC_PclkCfg(void)
//{
//    if ((APB_SYSCTRL->QdecCfg >> 15) & 1)
//        return;
//    uint32_t hclk = SYSCTRL_GetHClk();
//    uint32_t slowClk = SYSCTRL_GetSlowClk();
//    uint8_t div = hclk / slowClk;
//    if (hclk % slowClk)
//        div++;
//    if (!(div >> 4))
//        SYSCTRL_SetPClkDiv(div);
//}


void QdecTimerInit(void)
{
    SYSCTRL_ClearClkGateMulti((1 << SYSCTRL_ITEM_APB_QDEC));
    SYSCTRL_ReleaseBlock(SYSCTRL_ITEM_APB_QDEC);
    QDEC_TmrCfg(QDEC_CHX,QDEC_TMR_STOP,QDEC_TMR_RELOAD_UP_VALE);
    QDEC_SetCHxTmrCntA(QDEC_CHX,0x3000); 
    QDEC_SetCHxTmrCntB(QDEC_CHX,0xa000);
    QDEC_SetCHxTmrCntC(QDEC_CHX,0xF000);
    
    QDEC_SetChxIntEn(QDEC_CHX,1,0x1c);
}

void QdecPwmInit(void)
{
    SYSCTRL_ClearClkGateMulti((1 << SYSCTRL_ITEM_APB_QDEC));
    SYSCTRL_ReleaseBlock(SYSCTRL_ITEM_APB_QDEC);
    QDEC_ChModeCfg(QDEC_CHX,QDEC_PWM);
    QDEC_SetChxCpcTrg(QDEC_CHX,QDEC_TMR_RELOAD_UP_VALE);
    //mode1
    QDEC_SetCHxTmrCntA(QDEC_CHX,50); 
    QDEC_SetCHxTmrCntB(QDEC_CHX,70);
    QDEC_SetCHxTmrCntC(QDEC_CHX,100);
    QDEC_SetOutxEdge(QDEC_CHX,QDEC_CHX_ACPC,QDEC_EX_FALLING_EDGE);
    QDEC_SetOutxEdge(QDEC_CHX,QDEC_CHX_ACPA,QDEC_EX_RISING_EDGE);
    QDEC_SetOutxEdge(QDEC_CHX,QDEC_CHX_BCPB,QDEC_EX_FALLING_EDGE);
    QDEC_SetOutxEdge(QDEC_CHX,QDEC_CHX_BCPC,QDEC_EX_RISING_EDGE);
}

void QdecPwmGpioInit(void)
{
    SYSCTRL_ClearClkGateMulti((1 << SYSCTRL_ITEM_APB_GPIO0));
    PINCTRL_SetPadMux(GIO_GPIO_6,IO_SOURCE_QDEC_TIMER_EXT_OUT0_A);
    PINCTRL_SetPadMux(GIO_GPIO_9,IO_SOURCE_QDEC_TIMER_EXT_OUT0_B);
}

#define DMA_LEN     10
uint32_t QdecPcapData[DMA_LEN];
DMA_Descriptor pcap_dma __attribute__((aligned (8)));
int16_t out_data;
void QdecDmaInit(void)
{
    SYSCTRL_ClearClkGate(SYSCTRL_ITEM_APB_DMA);
     
    pcap_dma.Next = (DMA_Descriptor *)0;
    DMA_PreparePeripheral2Mem(&pcap_dma,QdecPcapData,SYSCTRL_DMA_QDEC0,DMA_LEN * 4,DMA_ADDRESS_INC,0);
    pcap_dma.SrcAddr = (uint32_t)&APB_QDEC->channels[QDEC_CHX].channel_write_b;
    DMA_EnableChannel(0, &pcap_dma);
}

void QdecPcapInit(void)
{
    SYSCTRL_ClearClkGateMulti((1 << SYSCTRL_ITEM_APB_QDEC));
    SYSCTRL_ReleaseBlock(SYSCTRL_ITEM_APB_QDEC);
    QDEC_ChModeCfg(QDEC_CHX,QDEC_PCM);
    QDEC_SetOutxEdge(QDEC_CHX,QDEC_CHX_ACPC,QDEC_EX_FALLING_EDGE);
    QDEC_SetOutxEdge(QDEC_CHX,QDEC_CHX_ACPA,QDEC_EX_RISING_EDGE);
    QDEC_EnableDMAChAB(QDEC_CHX,0,1);
    QdecDmaInit();
//    QDEC_SetChxIntEn(QDEC_CHX,1,1);
}

void QdecPcapGpioInit(void)
{
    SYSCTRL_ClearClkGateMulti((1 << SYSCTRL_ITEM_APB_GPIO0));
    PINCTRL_SelQDECPcapIn(IO_SOURCE_QDEC_PHASEA,GIO_GPIO_7);
}

void QdecStart(void)
{
    QDEC_EnableChannel(QDEC_CHX,0);
    QDEC_EnableChannel(QDEC_CHX,1);   
}

static uint32_t DMA_cb_isr(void *user_data)
{
    uint32_t state = DMA_GetChannelIntState(0);
    QDEC_EnableChannel(QDEC_CHX,0);
    DMA_ClearChannelIntState(0, state);
//    APB_DMA->Channels[0].Descriptor.DstAddr = (uint32_t)QdecPcapData;
//    APB_DMA->Channels[0].Descriptor.TranSize = DMA_LEN;
    DMA_EnableChannel(0, &pcap_dma);
    for(uint8_t i=0;i<DMA_LEN;i++)
    {
        printf("dma:0x%x\r\n",QdecPcapData[i]);
    }
    QDEC_EnableChannel(QDEC_CHX,1);
    

    return 0;
}

void QDEC0_IRQHandler(void)
{
    uint32_t *data;
    qdec0_callback(data);
}

void QDEC1_IRQHandler(void)
{
    uint32_t *data;
    qdec1_callback(data);
}

void QDEC2_IRQHandler(void)
{
    uint32_t *data;
    qdec2_callback(data);
}

void set_callback(void)
{
    NVIC_ConfigurationQdec0();
    NVIC_ConfigurationQdec1();
    NVIC_ConfigurationQdec2();
//    platform_set_irq_callback(PLATFORM_CB_IRQ_DMA,DMA_cb_isr,0);
//    platform_set_irq_callback(PLATFORM_CB_IRQ_QDEC0,qdec0_callback,0);
//    platform_set_irq_callback(PLATFORM_CB_IRQ_QDEC1,qdec0_callback,0);
//    platform_set_irq_callback(PLATFORM_CB_IRQ_QDEC2,qdec0_callback,0);
}

void QdecTimerTest()
{
    SYSCTRL_SelectQDECClk(SYSCTRL_CLK_SLOW, 100);
//    QDEC_PclkCfg();
    set_callback();
    QdecTimerInit();
    QdecStart();
}

void QdecPwmTest()
{
//    SYSCTRL_SelectQDECClk(SYSCTRL_CLK_SLOW, 10);
//    QDEC_PclkCfg();
    QdecPwmInit();
    QdecPwmGpioInit();
    QdecStart();
}

void QdecPcapTest()
{
    SYSCTRL_SelectQDECClk(SYSCTRL_CLK_SLOW, 10);
    QdecPcapGpioInit();
    QdecPcapInit();
    set_callback();
    QdecStart();
}

uint32_t a = 0;
uint8_t flg = 1;
void QdecTest(void)
{
    SYSCTRL_ClearClkGateMulti((1 << SYSCTRL_ITEM_APB_PinCtrl) |
                              (1 << SYSCTRL_ITEM_APB_QDEC));
    
//    QdecTimerTest();
    QdecPwmTest();
//    QdecPcapTest();
    while(1)
    {
//        if(flg)
//        {
//            a++;
//            if(a>99)
//               flg = 0; 
//        }
//        else
//        {
//            a--;
//            if(a<1)
//                flg = 1; 
//        }
        QDEC_SetCHxTmrCntA(QDEC_CHX,1);
        printf("get data:0x%x\r\n",APB_QDEC->channels[QDEC_CHX].channel_write_a);
        SYSCTRL_DelayCycles(24000,1000);
        
    }
}


