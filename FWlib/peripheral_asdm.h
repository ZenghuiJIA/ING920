#ifndef PERPHERAL_ASDM_H_
#define PERPHERAL_ASDM_H_
// #include <stdint.h>
#include "ingsoc.h"

typedef enum {
    ASDM_RX_FLG_EN           = (1 << 16),
    ASDM_FIFO_FULL_EN        = (1 << 17),
    ASDM_FIFO_EMPTY_EN       = (1 << 18),
}ASDM_FifoMask;

typedef enum {
    ASDM_DMIC_64_DOWN_SAMPLE = 0,
    ASDM_DMIC_128_DOWN_SAMPLE = 1,
}ASDM_DmicSample;

typedef enum {
    ASDM_SR_48k = 1,
    ASDM_SR_44k = (1<<1),
    ASDM_SR_32k = (1<<2),
    ASDM_SR_24k = (1<<3),
    ASDM_SR_22k = (1<<4),
    ASDM_SR_16k = (1<<5),
    ASDM_SR_12k = (1<<6),
    ASDM_SR_11k = (1<<7),
    ASDM_SR_8k  = (1<<8),
} ASDM_SampleRatek;

typedef struct 
{
    ASDM_DmicSample Dmic_sample;
    ASDM_SampleRatek Sample_rate;
    uint8_t INT_mask;
    uint8_t Fifo_trig_num;
}ASDM_PdmControlTypeDef;



void PDM_Enable(ASDM_TypeDef *base, uint8_t enable);
void ASDM_SetEdgeSample(ASDM_TypeDef *base, uint8_t edge);
void ASDM_SetHighPassEn(ASDM_TypeDef *base, uint8_t enable);
void ASDM_HighPassErrorEn(ASDM_TypeDef *base, uint8_t enable);
void ASDM_InvertInput(ASDM_TypeDef *base, uint8_t enable);
void ASDM_SelMic(ASDM_TypeDef *base, uint8_t enable);
void ASDM_SelDownSample(ASDM_TypeDef *base, uint8_t enable);
void ASDM_UpdateSr(ASDM_TypeDef *base, uint32_t div);
void ASDM_Reset(ASDM_TypeDef *base);
void ASDM_MuteEn(ASDM_TypeDef *base, uint8_t enable);
void ASDM_FadeInOutEn(ASDM_TypeDef *base, uint8_t enable);
void ASDM_SetVolStep(ASDM_TypeDef *base, uint32_t step);
uint8_t ADSM_VolZeroState(ASDM_TypeDef *base);
void ASDM_SetVol(ASDM_TypeDef *base, uint32_t vol);
uint32_t ASDM_GetOutData(ASDM_TypeDef *base);
void ASDM_SetHighPassFilter(ASDM_TypeDef *base, uint32_t data);
uint8_t ADSM_GetReadFlag(ASDM_TypeDef *base);
void ASDM_ClrOverflowError(ASDM_TypeDef *base);
void ASDM_AGCEn(ASDM_TypeDef *base, uint8_t enable);
void ASDM_SetAGCMaxLevel(ASDM_TypeDef *base, uint32_t level);
void ASDM_SetAGCFrameTime(ASDM_TypeDef *base, uint32_t time);
void ASDM_SetAGCNoiseThresh(ASDM_TypeDef *base, uint32_t noise);
void ASDM_SetAGCNoiseMode(ASDM_TypeDef *base, uint8_t mode);
void ASDM_AGCNoiseGateEn(ASDM_TypeDef *base, uint8_t enable);
void ASDM_SetAGCGateMode(ASDM_TypeDef *base, uint8_t mode);
void ASDM_NoiseHold(ASDM_TypeDef *base, uint8_t enable);
void ASDM_SetAGCMute(ASDM_TypeDef *base, uint8_t enable);
uint8_t ASDM_GetFifoFullState(ASDM_TypeDef *base);
uint8_t ASDM_GetFifoEmptyState(ASDM_TypeDef *base);
void ASDM_clrFifo(ASDM_TypeDef *base);
void ASDM_FifoEn(ASDM_TypeDef *base, uint8_t enable);
uint8_t ASDM_GetFifoCount(ASDM_TypeDef *base);
void ASDM_SetFifoDepth(ASDM_TypeDef *base, uint8_t depth);
void ASDM_SetFifoInt(ASDM_TypeDef *base, uint8_t items);
void ASDM_SetFifoTrig(ASDM_TypeDef *base, uint8_t trig);

#endif
