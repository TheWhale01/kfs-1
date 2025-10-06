#include "timer.h"
#include "idt.h"
#include "julo.h"
#include <stdint.h>

timer_t timer = {
    .ticks = 0,
    .freq = 100,
};

void init_timer(void) {
    uint32_t divisor = 1193180 / timer.freq;

    irq_install_handler(0, &timer_handler);

    // Setting up timer mode
    outb(CMD_REGISTER_PORT, OP_MODE_SQ_WAVE_GEN | LO_HIBYTE | CHANNEL_0);
    outb(CHANNEL_0_DATA_PORT, (uint8_t)(divisor & 0xFF));
    outb(CHANNEL_0_DATA_PORT, (uint8_t)((divisor >> 8) & 0xFF));
}

void timer_handler(int_regs_t *regs) {
    (void)regs;
    timer.ticks++;
}
