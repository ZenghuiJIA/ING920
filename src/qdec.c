#include "qdec.h"

#if 1
#include "SEGGER_RTT.h"
#include "debug_print.h"
#define DEBUG_LOG(...)      SEGGER_RTT_printf(0,##__VA_ARGS__)

#define DEBUG_UART_log      debug_uart
#else
#define DEBUG_LOG(...)
#define DEBUG_UART_log(...)
#endif

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
void qudec_test(void)
{
    // setup qdec
    SYSCTRL_ClearClkGateMulti((1 << SYSCTRL_ITEM_APB_PinCtrl) |
                              (1 << SYSCTRL_ITEM_APB_QDEC));
    SYSCTRL_ReleaseBlock(SYSCTRL_ITEM_APB_QDEC);
    PINCTRL_SelQDECIn(9, 10);    // set GPIO16=phase_a, GPIO17=phase_b

    SYSCTRL_SelectQDECClk(SYSCTRL_CLK_SLOW, 100);
//    QDEC_PclkCfg();    // set pclk not bigger than sclk_slow
    QDEC_EnableQdecDiv(QDEC_DIV_1024);
    QDEC_QdecCfg(50, 1);
    QDEC_ChannelEnable(1);

    // print qdec data and direction when rotate the mouse wheel manually
    uint16_t preData = 0;
    uint16_t data = 0;
    uint8_t dir;
    while(1) {
        data = QDEC_GetData();
        dir = QDEC_GetDirection();
        if (data != preData) {
            if (dir) {
                DEBUG_UART_log("data: %d, %s\n", data, "anticlockwise");
            } else {
                DEBUG_UART_log("data: %d, %s\n", data, "clockwise");
            }
        }
        preData = data;
    }

}