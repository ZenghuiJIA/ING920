#include "qdec_timer.h"
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


void QdecTimerInit(void)
{
    SYSCTRL_ClearClkGateMulti((1 << SYSCTRL_ITEM_APB_QDEC));
    SYSCTRL_ReleaseBlock(SYSCTRL_ITEM_APB_QDEC);
    QDEC_ChModeCfg(QDEC_CH0,QDEC_TIMER);
    QDEC_TmrCfg(QDEC_CH0,QDEC_TMR_RELOAD,QDEC_TMR_RELOAD_UP_FULL);
    QDEC_SetCHxTmrCntA(QDEC_CH0,0x3000);
    QDEC_SetCHxTmrCntA(QDEC_CH0,0xa000);
    QDEC_SetCHxTmrCntA(QDEC_CH0,0xF000);
    QDEC_SetChxIntEn(QDEC_CH0,1,0x1c);
}

void QdecTest(void)
{
    QdecTimerInit();
//    NVIC_ConfigurationQdec0();
//    QDEC_EnableChannel(QDEC_CH0,1);
    while(1)
    {
        
    }
}


void QDEC2_IRQHandler(void)
{
    uint32_t state = QDEC_GetChxIntStateClr(QDEC_CH0);
    DEBUG_LOG("state:0x%x\r\n",state);
    if(state&QDEC_INT_BUF_FULL_STATE)
    {
        DEBUG_LOG("QDEC_INT_BUF_FULL_STATE\r\n");
    }
    if(state&QDEC_INT_PDC_END_STATE)
    {
        DEBUG_LOG("QDEC_INT_PDC_END_STATE\r\n");
    }
    if(state&QDEF_INT_ETRGS_CMB_STATE)
    {
        DEBUG_LOG("QDEF_INT_ETRGS_CMB_STATE\r\n");
    }
    if(state&QDEC_INT_LDRBS_CMB_STATE)
    {
        DEBUG_LOG("QDEC_INT_LDRBS_CMB_STATE\r\n");
    }
    if(state&QDEC_INT_LDRAS_CMB_STATE)
    {
        DEBUG_LOG("QDEC_INT_LDRAS_CMB_STATE\r\n");
    }
    if(state&QDEC_INT_CPCS_CMB_STATE)
    {
        DEBUG_LOG("QDEC_INT_CPCS_CMB_STATE\r\n");
    }
    if(state&QDEC_INT_CPBS_CMB_STATE)
    {
        DEBUG_LOG("QDEC_INT_CPBS_CMB_STATE\r\n");
    }
    if(state&QDEC_INT_CPAS_CMB_STATE)
    {
        DEBUG_LOG("QDEC_INT_CPAS_CMB_STATE\r\n");
    }
    if(state&QDEC_INT_LOVFS_CMB_STATE)
    {
        DEBUG_LOG("QDEC_INT_LOVFS_CMB_STATE\r\n");
    }
    if(state&QDEC_INT_COVFS_CMB_STATE)
    {
        DEBUG_LOG("QDEC_INT_COVFS_CMB_STATE\r\n");
    }
}

void QDEC1_IRQHandler(void)
{
    uint32_t state = QDEC_GetChxIntStateClr(QDEC_CH0);
    DEBUG_LOG("state:0x%x\r\n",state);
    if(state&QDEC_INT_BUF_FULL_STATE)
    {
        DEBUG_LOG("QDEC_INT_BUF_FULL_STATE\r\n");
    }
    if(state&QDEC_INT_PDC_END_STATE)
    {
        DEBUG_LOG("QDEC_INT_PDC_END_STATE\r\n");
    }
    if(state&QDEF_INT_ETRGS_CMB_STATE)
    {
        DEBUG_LOG("QDEF_INT_ETRGS_CMB_STATE\r\n");
    }
    if(state&QDEC_INT_LDRBS_CMB_STATE)
    {
        DEBUG_LOG("QDEC_INT_LDRBS_CMB_STATE\r\n");
    }
    if(state&QDEC_INT_LDRAS_CMB_STATE)
    {
        DEBUG_LOG("QDEC_INT_LDRAS_CMB_STATE\r\n");
    }
    if(state&QDEC_INT_CPCS_CMB_STATE)
    {
        DEBUG_LOG("QDEC_INT_CPCS_CMB_STATE\r\n");
    }
    if(state&QDEC_INT_CPBS_CMB_STATE)
    {
        DEBUG_LOG("QDEC_INT_CPBS_CMB_STATE\r\n");
    }
    if(state&QDEC_INT_CPAS_CMB_STATE)
    {
        DEBUG_LOG("QDEC_INT_CPAS_CMB_STATE\r\n");
    }
    if(state&QDEC_INT_LOVFS_CMB_STATE)
    {
        DEBUG_LOG("QDEC_INT_LOVFS_CMB_STATE\r\n");
    }
    if(state&QDEC_INT_COVFS_CMB_STATE)
    {
        DEBUG_LOG("QDEC_INT_COVFS_CMB_STATE\r\n");
    }
}

void QDEC0_IRQHandler(void)
{
    uint32_t state = QDEC_GetChxIntStateClr(QDEC_CH0);
    DEBUG_LOG("state:0x%x\r\n",state);
    if(state&QDEC_INT_BUF_FULL_STATE)
    {
        DEBUG_LOG("QDEC_INT_BUF_FULL_STATE\r\n");
    }
    if(state&QDEC_INT_PDC_END_STATE)
    {
        DEBUG_LOG("QDEC_INT_PDC_END_STATE\r\n");
    }
    if(state&QDEF_INT_ETRGS_CMB_STATE)
    {
        DEBUG_LOG("QDEF_INT_ETRGS_CMB_STATE\r\n");
    }
    if(state&QDEC_INT_LDRBS_CMB_STATE)
    {
        DEBUG_LOG("QDEC_INT_LDRBS_CMB_STATE\r\n");
    }
    if(state&QDEC_INT_LDRAS_CMB_STATE)
    {
        DEBUG_LOG("QDEC_INT_LDRAS_CMB_STATE\r\n");
    }
    if(state&QDEC_INT_CPCS_CMB_STATE)
    {
        DEBUG_LOG("QDEC_INT_CPCS_CMB_STATE\r\n");
    }
    if(state&QDEC_INT_CPBS_CMB_STATE)
    {
        DEBUG_LOG("QDEC_INT_CPBS_CMB_STATE\r\n");
    }
    if(state&QDEC_INT_CPAS_CMB_STATE)
    {
        DEBUG_LOG("QDEC_INT_CPAS_CMB_STATE\r\n");
    }
    if(state&QDEC_INT_LOVFS_CMB_STATE)
    {
        DEBUG_LOG("QDEC_INT_LOVFS_CMB_STATE\r\n");
    }
    if(state&QDEC_INT_COVFS_CMB_STATE)
    {
        DEBUG_LOG("QDEC_INT_COVFS_CMB_STATE\r\n");
    }
}
