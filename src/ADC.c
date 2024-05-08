#include "ADC.h"
#include "peripheral_adc.h"
#include "peripheral_gpio.h"
#include "peripheral_sysctrl.h"

#define ADC_CHANNEL    ADC_CH_0
#define ADC_CLK_MHZ    6

static uint32_t ADC_cb_isr(void *user_data)
{
    uint32_t data = ADC_PopFifoData();
    SADC_channelId channel = ADC_GetDataChannel(data);
    if (channel == ADC_CHANNEL) {
        uint16_t sample = ADC_GetData(data);
        // do something with 'sample'
    }
    return 0;
}

void adc_init(void)
{
    SYSCTRL_ClearClkGate(SYSCTRL_ITEM_APB_ADC);
    SYSCTRL_SetAdcClkDiv(24 / ADC_CLK_MHZ);
    SYSCTRL_ReleaseBlock(SYSCTRL_ITEM_APB_ADC);
    ADC_Reset();
    //now adc calibration cant be used
//    ADC_ftInit();
//    ADC_Calibration(SINGLE_END_MODE);
    ADC_ConvCfg(SINGLE_MODE, PGA_PARA_4, 1, ADC_CHANNEL, 1, 0, SINGLE_END_MODE, 0);
    ADC_Start(1);
}

