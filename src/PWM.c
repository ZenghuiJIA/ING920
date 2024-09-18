#include "PWM.h"
#include "misc.h"
#include "peripheral_sysctrl.h"
#include "peripheral_dma.h"

#define PWM_DEBUG
#ifdef PWM_DEBUG
#include "SEGGER_RTT.h"
#include "debug_print.h"
#define DEBUG_LOG(...)      SEGGER_RTT_printf(0,##__VA_ARGS__)

#define DEBUG_UART_log      debug_uart
#else
#define DEBUG_LOG(...)
#define DEBUG_UART_log(...)
#endif

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

static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
  
    NVIC_InitStructure.NVIC_IRQChannel = PWM0_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = 1;
    /* init NVIC */
    NVIC_Init(&NVIC_InitStructure);
}

//pwm输入捕获
void PCP_gpio_init(void)
{
    SYSCTRL_ClearClkGateMulti((1 << SYSCTRL_ClkGate_APB_PinCtrl) | (1 << SYSCTRL_ClkGate_APB_GPIO0));
    SYSCTRL_EnablePcapMode(0,1);
    SYSCTRL_ReleaseBlock(SYSCTRL_ClkGate_APB_PWM);
    SYSCTRL_ClearClkGate(SYSCTRL_ClkGate_APB_PWM);
    PCAP_Enable(0);
    PCAP_EnableEvents(0,
                      PCAP_PULSE_FALLING_EDGE|PCAP_PULSE_RISING_EDGE,
                      PCAP_PULSE_FALLING_EDGE|PCAP_PULSE_RISING_EDGE);

    PINCTRL_SelPCAPIn(0, GIO_GPIO_9);
    PINCTRL_SelPCAPIn(1, GIO_GPIO_10);
    NVIC_Configuration();
    PWM_FifoIntEnable(0,1,PWM_FIFO_FULL_EN | PWM_FIFO_TRIGGER_EN);
    PCAP_CounterEnable(1);
    PWM_Enable(0, 1);
}

uint32_t  IR_send_buff[30] = {0x8008, 0x0008};
void IR_Cycle_DMA_Init(void)
{
    SYSCTRL_ClearClkGate(SYSCTRL_ITEM_APB_DMA);
    
    APB_DMA->Channels[0].Descriptor.Next = (DMA_Descriptor*)&APB_DMA->Channels[0];
    //配置DMA发送
    SYSCTRL_SelectUsedDmaItems(1<<SYSCTRL_DMA_PWM0);
    DMA_PrepareMem2Peripheral(&APB_DMA->Channels[0].Descriptor,SYSCTRL_DMA_PWM0,IR_send_buff,30,DMA_ADDRESS_INC,0);
    DMA_EnableChannel(0, &APB_DMA->Channels[0].Descriptor);
}

static int buff_cnt = 0;
//void IR_WriteCarrierData(uint8_t channel, uint32_t data)
//{
//    if(buff_cnt > 30)
//        buff_cnt = 0;
//    IR_send_buff[buff_cnt] = data;
//    buff_cnt++;
//}

//uint8_t IR_BusyState(uint8_t channel)
//{
////    uint8_t state;

////    state = APB_PWM->Channels[channel].Ctrl1 & (1<<2);

//    return 0;
//}

#define IR_REG_SW   (1<<15)
#define NEC_START_P_NUM 342
#define NEC_START_I_NUM 172
#define NEC_LOGIC_NUM 22
#define NEC_STOP_NUM  22
#define NEC_START()  do { \
    IR_WriteCarrierData(0,NEC_START_P_NUM);\
    while (IR_BusyState(0));\
    IR_WriteCarrierData(0,NEC_START_I_NUM | IR_REG_SW);\
    while (IR_BusyState(0));\
}while(0)

#define NEC_LOGIC(x) do { \
    uint8_t pulse;         \
    IR_WriteCarrierData(0,NEC_LOGIC_NUM);\
    while (IR_BusyState(0));  \
    pulse = NEC_LOGIC_NUM*(x * 3 + 1);   \
    IR_WriteCarrierData(0,pulse | IR_REG_SW);\
    while (IR_BusyState(0));\
}while(0)
#define NEC_STOP()  do { \
    IR_WriteCarrierData(0,NEC_STOP_NUM);\
    while (IR_BusyState(0));\
}while(0)


void IR_Write_data(uint8_t  data)
{
    uint8_t  i;
    for(i = 0; i<8; i++)
    {
        NEC_LOGIC((data & (1 << i)));
    }
}

void IR_NECSend_Command(uint8_t address, uint8_t cmd)
{
    buff_cnt = 0;
    NEC_START();
    IR_Write_data(address);
    IR_Write_data(~address);
    IR_Write_data(cmd);
    IR_Write_data(~cmd);
    NEC_STOP();
}

//ir输出对应载波
void IR_out_init(void)
{
    SYSCTRL_EnablePcapMode(0,0);
	PWM_SetMask(0,0,0);//设置通道输出极性
    SYSCTRL_ReleaseBlock(SYSCTRL_ClkGate_APB_PWM);
    SYSCTRL_ClearClkGate(SYSCTRL_ClkGate_APB_PWM);
    PWM_SetMode(0, 7);
    PWM_StepEnabled(0,0);//close step enabled
    //配置两组寄存器对应频率占空比，通常情况一组为载波一组为0或1
    IR_CycleCarrierSetup(0,0,38000,70);
    IR_CycleCarrierSetup(0,1,38000,0);
//    IR_Cycle_DMA_Init();
//    IR_NECSend_Command(0x32, 0x55);
//    PWM_DmaEnable(0, 2, 1);
    PWM_Enable(0, 1);
    
}


//NEC协议发送测试cycle模式直接写寄存器操作
void IR_test_cycle(void)
{
    pwn_simple_init();
    IR_out_init();
    while(1){
        IR_NECSend_Command(0x32, 0x55);
//        IR_Cycle_DMA_Init();
        //DEBUG_UART_log("SEND IR\r\n");
        SYSCTRL_DelayCycles(2000,10000);
    }
}
//pwm呼吸灯step步进
void pwm_test_simple(void)
{
    pwn_simple_init();
    PWM_BreathingLightInit(0,10000,50);
//    PWM_SetupSingle(0,50);
    while(1){
        SYSCTRL_DelayCycles(1000,10);
    }
}

//pwm生成方波
void pwm_test_step(void)
{
    pwn_simple_init();
    PWM_SetupSimpleInit(0,240,50,0);
    while(1){
        SYSCTRL_DelayCycles(1000,10);
    }
}
uint8_t read_count;
#define GET_MSK(x)      ((1<<x) - 1)

uint32_t read_cap_value;
void pcap_test_read(void)
{
    uint32_t data;
    PCP_gpio_init();
    while(1){
//        read_count = (APB_PWM->Channels[0].Ctrl1 & (GET_MSK(3) << 4)) >> 4;
//        DEBUG_UART_log("read count: %d\r\n",read_count);
//        for(uint8_t i = read_count; i > 0; i--){
//            data = APB_PWM->PCAPChannels[0].Ctrl1;
//            DEBUG_UART_log("read data: %d\r\n",read_count);
//        }
//        read_cap_value = PCAP_ReadCounter();
        SYSCTRL_DelayCycles(1000,10);
    }

}


void PWM0_IRQHandler(void)
{
    uint32_t stat_,num;
    PWM_FIFO_STATUS_t stat;
    int m, n, p;
    
    /* Get FIFO cnt for three channels */
    stat_ = PWM_GetFifoStatus(0);
    stat = *(PWM_FIFO_STATUS_t*)&stat_;
    m = stat.fifo_cnt;
    DEBUG_UART_log("read count: %x\n",m);
    for(;m>0; m--){
        num = PCAP_ReadData(0);
        DEBUG_UART_log("read num: %x\n",num);
    }

}
