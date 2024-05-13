#include "PWM.h"

void pwn_simple_init(void)
{
    SYSCTRL_ClearClkGateMulti((1 << SYSCTRL_ClkGate_APB_PWM)
                              | (1 << SYSCTRL_ClkGate_APB_PinCtrl) | (1 << SYSCTRL_ClkGate_APB_GPIO0));
    PINCTRL_SetPadMux(GIO_GPIO_9, IO_SOURCE_PWM0_B);
    PINCTRL_SetPadMux(GIO_GPIO_10, IO_SOURCE_PWM0_A);
}

void PWM_SetupSimpleInit(const uint8_t channel_index, const uint32_t frequency, const uint16_t on_duty, const uint16_t died_duty)
{
    uint32_t pera = PWM_CLOCK_FREQ / frequency;
    uint32_t high = pera > 1000 ? pera / 100 * (100 - on_duty) : pera * (100 - on_duty) / 100;
    uint32_t died = pera > 1000 ? pera / 100 * (died_duty) : pera * (died_duty) / 100;
    PWM_HaltCtrlEnable(channel_index, 1);
    PWM_Enable(channel_index, 0);
    PWM_SetPeraThreshold(channel_index, pera);
    PWM_SetHighThreshold(channel_index, 0, high);
    PWM_SetDiedZoneThreshold(channel_index,died);
    PWM_SetMode(channel_index, PWM_WORK_MODE_UP_WITH_DIED_ZONE);
    PWM_SetMask(channel_index, 0, 0);
    PWM_Enable(channel_index, 1);
    PWM_HaltCtrlEnable(channel_index, 0);
}

void PWM_BreathingLightInit(const uint8_t channel_index, const uint32_t frequency, const uint16_t on_duty)
{
    uint32_t pera = PWM_CLOCK_FREQ / frequency;
    uint32_t high = pera > 1000 ? pera / 100 * (100 - on_duty) : pera * (100 - on_duty) / 100;
    PWM_HaltCtrlEnable(channel_index, 1);
    PWM_Enable(channel_index, 0);
    PWM_SetPeraThreshold(channel_index, pera);
    PWM_SetHighThreshold(channel_index, 0, high);
    PWM_SetStepCnt(0, 1);
    PWM_StepLoopEnabled(0, 1);
    PWM_StepEnabled(0, 1);
    PWM_SetMode(channel_index, PWM_WORK_MODE_UP_WITH_DIED_ZONE);
    PWM_SetMask(channel_index, 0, 0);
    PWM_Enable(channel_index, 1);
    PWM_HaltCtrlEnable(channel_index, 0);
}

void pwm_test_simple(void)
{
    pwn_simple_init();
//    PWM_SetupSimpleInit(0,1000,15,3);
    PWM_BreathingLightInit(0,1000,10);
//    PWM_SetupSingle(0,50);
    while(1){
        SYSCTRL_DelayCycles(1000,10);
//        PWM_SetupSingle(0,50);
        ;
    }
}
