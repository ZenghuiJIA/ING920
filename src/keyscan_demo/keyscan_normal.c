#include "keyscan_normal.h"
//该demo演示标准矩阵按键的扫描处理过程，normal模式中断触发数据处理
#define keyscan_DEBUG
#ifdef keyscan_DEBUG
#include "SEGGER_RTT.h"
#include "debug_print.h"
#define DEBUG_LOG(...)      SEGGER_RTT_printf(0,##__VA_ARGS__)

#define DEBUG_UART_log      debug_uart
#else
#define DEBUG_LOG(...)
#define DEBUG_UART_log(...)
#endif

KEYSCAN_OutRowList key_out_row[] = {
        {KEY_OUT_ROW_0, GIO_GPIO_24}, // 第1行
        {KEY_OUT_ROW_1, GIO_GPIO_25}, // 第2行
        {KEY_OUT_ROW_2, GIO_GPIO_26}, // 第3行
        {KEY_OUT_ROW_3, GIO_GPIO_27}, // 第4行
};
#define key_out_row_num (sizeof(key_out_row) / sizeof(key_out_row[0]))

KEYSCAN_InColList key_in_col[] = {
        {KEY_IN_COL_0, GIO_GPIO_12}, // 第1列
        {KEY_IN_COL_1, GIO_GPIO_13}, // 第2列
        {KEY_IN_COL_6, GIO_GPIO_18}, // 第3列
        {KEY_IN_COL_7, GIO_GPIO_19}, // 第4列
};
#define key_in_col_num (sizeof(key_in_col) / sizeof(key_in_col[0]))

static KEYSCAN_Ctx key_ctx = {0};

static KEYSCAN_SetStateStruct keyscan_set = {
        .col               = key_in_col,
        .col_num           = key_in_col_num,
        .row               = key_out_row,
        .row_num           = key_out_row_num,
        .loop_num_trig_int = 0,
        .int_loop_en       = 0,
        .table_mode_en     = 0,
        .fifo_num_trig_int = 10,
        .release_time      = 150,
        .scan_interval     = 170,
        .debounce_counter  = 50,
        .dma_num_trig_int  = 0x10,
        .dma_en            = 0,
        .int_trig_en       = 1,
        .lpkey_mode_en	   = 0,
};

static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = KeyScan_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = 1;
    /* init NVIC */
    NVIC_Init(&NVIC_InitStructure);
}

void key_scan_normal_test()
{
    SYSCTRL_ClearClkGateMulti(1 << SYSCTRL_ITEM_APB_KeyScan);
    SYSCTRL_ClearClkGateMulti(1 << SYSCTRL_ITEM_APB_PinCtrl);
    NVIC_Configuration();
    KEYSCAN_Initialize(&keyscan_set);
    //KEYSCAN_SetInColMask(0x00);
    //KEYSCAN_InitKeyScanToIdx(&keyscan_set, &key_ctx);
    while(1)
    {
        SYSCTRL_DelayCycles(1000,500);
//        KEYSCAN_SetScannerEn(1);
    }
}
static uint16_t key_vale[10];
//保存所有按键键值并对比是否有更新
void key_scan_check(uint32_t in_key_vale)
{
    static uint16_t num = 0;

    key_vale[num] = in_key_vale;
    num++;
    if(num > 9 || in_key_vale == 0x400){
        DEBUG_LOG("-----------------------\r\n");
        for(uint8_t i = 0; i<10;i++)
        {
            key_vale[i] = 0;
        }
        num = 0;
    }
}

void KeyScan_IRQHandler(void)
{
    KEYSCAN_LoopIntClr();
//    KEYSCAN_SetScannerEn(0);
    uint16_t key_data;
    while (KEYSCAN_GetIntStateFifoEmptyRaw() == 0) {
        key_data = KEYSCAN_GetKeyData();
        DEBUG_LOG("key data trg:0x%x\n",key_data);
        DEBUG_UART_log("1\r\n");
        key_scan_check(key_data);
    }
}
