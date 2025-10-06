#pragma once
#include "idt.h"
#include <stdint.h>
#include <stddef.h>

typedef enum {
    CHANNEL_0 = 0b00,
    CHANNEL_1 = 0b01,
    CHANNEL_2 = 0b10,
    READ_BACK_CMD = 0b11,
    LATCH_COUNT_CMD = 0b00,
    LOBYTE_ONLY = 0b01,
    HIBYTE_ONLY = 0b10,
    LO_HIBYTE = 0b11,
    OP_MODE_TERM_COUNT = 0b000,
    OP_MODE_HARDWARE_ONE_SHOT = 0b001,
    OP_MODE_RATE_GEN = 0b010,
    OP_MODE_SQ_WAVE_GEN = 0b011,
    OP_MODE_SOFT_TR = 0b100,
    OP_MODE_HARD_TR = 0b101,
} pit_modes_e;

typedef enum {
    CHANNEL_0_DATA_PORT = 0x40,
    CHANNEL_1_DATA_PORT = 0x41,
    CHANNEL_2_DATA_PORT = 0x42,
    CMD_REGISTER_PORT = 0x43,
} pit_pots_e;

typedef struct  {
    uint64_t ticks;
    const uint32_t freq;
} timer_t;

extern timer_t timer;

void init_timer(void);
void timer_handler(int_regs_t *regs);
