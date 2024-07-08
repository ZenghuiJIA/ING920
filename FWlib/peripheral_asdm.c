#include "peripheral_asdm.h"

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
