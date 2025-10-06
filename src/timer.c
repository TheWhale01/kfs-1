#include "timer.h"
#include "idt.h"
#include "julo.h"
#include "libft.h"
#include "signal.h"
#include <stdint.h>

scheduled_signal_list_t scheduled_signals = {
    .size = 0,
};

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
    if (scheduled_signals.size == 0)
        return ;
    for (size_t i = 0; i < scheduled_signals.size; i++) {
        if (scheduled_signals.queue[i].deliver_tick <= timer.ticks) {
            if (add_signal(scheduled_signals.queue[i].int_nb, scheduled_signals.queue[i].data)) {
                ft_memmove(&scheduled_signals.queue[i], &scheduled_signals.queue[i + 1], sizeof(scheduled_signals.queue) * (scheduled_signals.size - i - 1));
                scheduled_signals.size--;
                i--;
            }
        }
    }
}
