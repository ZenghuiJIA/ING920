#include "PDM.h"
#include "misc.h"

#define MAIN_DEBUG
#ifdef MAIN_DEBUG
#include "SEGGER_RTT.h"
#include "debug_print.h"
#define DEBUG_LOG(...)      SEGGER_RTT_printf(0,##__VA_ARGS__)
#define DEBUG_UART_log      debug_uart
#else
#define DEBUG_LOG(...)
#define DEBUG_UART_log(...)
#endif

static void set_reg_bits(volatile uint32_t *reg, uint32_t v, uint8_t bit_width, uint8_t bit_offset)
{
    uint32_t mask = ((1 << bit_width) - 1) << bit_offset;
    *reg = (*reg & ~mask) | (v << bit_offset);
}

static void set_reg_bit(volatile uint32_t *reg, uint8_t v, uint8_t bit_offset)
{
    uint32_t mask = 1 << bit_offset;
    *reg = (*reg & ~mask) | (v << bit_offset);
}

static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
  
    NVIC_InitStructure.NVIC_IRQChannel = DMA_SET_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = 1;
    /* init NVIC */
    NVIC_Init(&NVIC_InitStructure);
}

void pdm_io_init(void)
{
    SYSCTRL_ClearClkGate(SYSCTRL_ClkGate_APB_GPIO0);//gpio0
    SYSCTRL_ClearClkGate(SYSCTRL_ClkGate_APB_PinCtrl);
    PINCTRL_SelAsdm(GIO_GPIO_9,GIO_GPIO_10);
}

void pdm_init(void)
{
    //pclk_in
    set_reg_bit((uint32_t*)&(APB_SYSCTRL->CguCfg[4]), 0, 28);//asdm_posedge_sample
    //enable clk
    SYSCTRL_ClearClkGate(SYSCTRL_ITEM_APB_ASDM);
//    *((uint32_t*) 0x40100004) |= 1<<6;
    *((uint32_t*) 0x40102028) |= 1;

    //fclk clk
    //if use pull 30 - 12.801MHz, 34 - 11.295MHz 
    //fpga use 24m 1 12m
    //更新模块fclk时钟10分频
    SYSCTRL_UpdateAsdmClk(9);
    set_reg_bit(&APB_ASDM->asdm_ctrl, 1, 2);//asdm_posedge_sample
    set_reg_bit(&APB_ASDM->asdm_ctrl, 1, 3);//asdm_hpf_en
    set_reg_bit(&APB_ASDM->asdm_ctrl, 0, 4);//asdm_hpf_fb_dis
    set_reg_bit(&APB_ASDM->asdm_ctrl, 1, 5);//asdm_l_inv
    set_reg_bit(&APB_ASDM->asdm_ctrl, 1, 6);//asdm_dmic_sel
    set_reg_bit(&APB_ASDM->asdm_ctrl, 0, 7);//asdm_downsample_sel

    set_reg_bits(&APB_ASDM->asdm_sr, 1<<7, 9, 0);//asdm_sr fs 12k 
    set_reg_bit(&APB_ASDM->asdm_sr, 1, 9);//asdm_sr_update

    set_reg_bit(&APB_ASDM->asdm_mute_ctrl, 0, 0);//asdm_mute_l
    set_reg_bit(&APB_ASDM->asdm_mute_ctrl, 0, 1);//asdm_fadedis_l
    set_reg_bits(&APB_ASDM->asdm_mute_ctrl, 3, 8, 2);//asdm_vol_step

    set_reg_bits(&APB_ASDM->asdm_vol_ctrl, 0x3fff, 14, 0);//asdm_vol_l

    set_reg_bits(&APB_ASDM->asdm_hpf_coef, 0xffe, 12, 2);//asdm_hpf_coef

    set_reg_bit(&APB_ASDM->asdm_agc_ctrl0, 1, 0);//asdm_agc_sel
    set_reg_bits(&APB_ASDM->asdm_agc_ctrl0, 1<<4, 5, 1);//asdm_max_level

    set_reg_bits(&APB_ASDM->asdm_agc_ctrl1, 8, 12, 0);//asdm_frame_time

    set_reg_bits(&APB_ASDM->asdm_agc_ctrl2, 0xf, 5, 0);//asdm_noise_th
    set_reg_bit(&APB_ASDM->asdm_agc_ctrl2, 0, 5);//asdm_noise_mode
    set_reg_bit(&APB_ASDM->asdm_agc_ctrl2, 1, 6);//asdm_noise_gt_en
    set_reg_bit(&APB_ASDM->asdm_agc_ctrl2, 0, 7);//asdm_noise_gt_mode
    set_reg_bits(&APB_ASDM->asdm_agc_ctrl2, 0x7, 5, 8);//asdm_noise_hold

    set_reg_bit(&APB_ASDM->asdm_agc_st, 0, 0);//asdm_agc_mute

    set_reg_bit(&APB_ASDM->ckgt_ctrl, 0, 8);//cic_din_l_bypass
    set_reg_bit(&APB_ASDM->ckgt_ctrl, 0, 9);//cic_din_l_clk_sample_posedge
    set_reg_bits(&APB_ASDM->ckgt_ctrl, 0xAAA, 12, 16);
    
    set_reg_bit(&APB_ASDM->fifo_addr, 1, 3);//fifo enable
    set_reg_bit(&APB_ASDM->fifo_addr, 0, 16);//int_rx_flg_en
    set_reg_bit(&APB_ASDM->fifo_addr, 0, 17);//int_fifo_full_en
    set_reg_bit(&APB_ASDM->fifo_addr, 0, 18);//int_fifo_empty_en
    set_reg_bits(&APB_ASDM->fifo_addr, 3, 3, 19);//fifo_trig
    
    set_reg_bit(&APB_ASDM->asdm_ctrl, 1, 0);//DIG_en
    set_reg_bit(&APB_ASDM->asdm_ctrl, 1, 1);//dig_l_en
    
//    SYSCTRL_Update_sdm_clk(1);

}

void disable_pdm()
{
    set_reg_bit(&APB_ASDM->asdm_ctrl, 0, 0);//DIG_en
    set_reg_bit(&APB_ASDM->asdm_ctrl, 0, 1);//dig_l_en
}
#define DMA_LEN     1024
uint32_t mda_rc_pdm_data[DMA_LEN];
DMA_Descriptor pdm_dma __attribute__((aligned (8)));
uint8_t dma_data;
int16_t out_data;
void pdm_dma_init(void)
{
    NVIC_Configuration();
    SYSCTRL_ClearClkGate(SYSCTRL_ITEM_APB_DMA);
    
    SYSCTRL_SelectUsedDmaItems(
    (1<<SYSCTRL_DMA_UART0_RX) | (1<<SYSCTRL_DMA_UART1_RX) | (1<<SYSCTRL_DMA_SPI0_TX) | (1<<SYSCTRL_DMA_SPI1_TX) | 
    (1<<SYSCTRL_DMA_I2C0) | (1<<SYSCTRL_DMA_QDEC0) | (1<<SYSCTRL_DMA_PWM1) | (1<<SYSCTRL_DMA_I2S_RX) | 
    (1<<SYSCTRL_DMA_UART0_TX) | (1<<SYSCTRL_DMA_UART1_TX) | (1<<SYSCTRL_DMA_SPI0_RX) | (1<<SYSCTRL_DMA_SPI1_RX)| 
    (1<<SYSCTRL_DMA_QDEC1) | (1<<SYSCTRL_DMA_KeyScan) | (1<<SYSCTRL_DMA_I2S_TX) | (1<<SYSCTRL_DMA_SDADC_RX));
    
//    pdm_dma.Next = &pdm_dma;
//    DMA_PrepareMem2Mem(&APB_DMA->Channels[0].Descriptor,mda_rc_pdm_data,in_data,80,DMA_ADDRESS_INC,DMA_ADDRESS_INC,0);
    //配置DMA发送
    pdm_dma.Ctrl |= 2<<24;
    DMA_PreparePeripheral2Mem(&pdm_dma,mda_rc_pdm_data,SYSCTRL_DMA_SDADC_RX,DMA_LEN * 4,DMA_ADDRESS_INC,0);;
    DMA_EnableChannel(0, &pdm_dma);
}

void DMA_IRQHandler(void)
{
    uint32_t state = DMA_GetChannelIntState(0);
    DMA_ClearChannelIntState(0, state);
    
    dma_data = 1;
    DMA_EnableChannel(0, &pdm_dma);
}

void ASDM_IRQHandler(void)
{
    dma_data = 1;
}

#define ANO     0

void pdm_test(void)
{
    pdm_io_init();
    pdm_init();
//    NVIC_Configuration();
    pdm_dma_init();
//    disable_pdm();
    while(1)
    {
        if(dma_data)
        {
            for(uint32_t i = 0; i<DMA_LEN;i++)
            {
                out_data = (mda_rc_pdm_data[i] & 0xffff);
                #if ANO
                ano_output(&out_data,2);
                #else                
                debug_hex(&out_data,2);   
                #endif
                out_data = (mda_rc_pdm_data[i] >> 16);
                #if ANO
                ano_output(&out_data,2);
                #else                
                debug_hex(&out_data,2);   
                #endif
            }
            dma_data = 0;
        }
//        test_ano();
//            set_reg_bit(&APB_ASDM->fifo_addr, 1, 2);
    }
}
