#include "signal.h"
#include "libft.h"
#include "timer.h"

void init_signal(void) {
    signals.size = 0;
    scheduled_signals.size = 0;
    ft_bzero(signals.queue, SIGNAL_QUEUE_SIZE);
    ft_bzero(scheduled_signals.queue, SIGNAL_QUEUE_SIZE);
}

bool add_signal(size_t int_nb, void *data) {
    if (signals.size >= SIGNAL_QUEUE_SIZE)
        return false;
    signals.queue[signals.size].data = data;
    signals.queue[signals.size].int_nb = int_nb;
    signals.size++;
    return true;
}

void schedule_signal(size_t int_nb, void *data, uint64_t delay_ticks) {
    if (scheduled_signals.size >= SIGNAL_QUEUE_SIZE)
        return;
    scheduled_signals.queue[scheduled_signals.size].int_nb = int_nb;
    scheduled_signals.queue[scheduled_signals.size].data = data;
    scheduled_signals.queue[scheduled_signals.size].deliver_tick = timer.ticks + delay_ticks;
    scheduled_signals.size++;
}
