#include "signal.h"
#include "libft.h"

void init_signal(void) {
    signals.head = 0;
    signals.tail = 0;
    ft_bzero(signals.queue, SIGNAL_QUEUE_SIZE);
}
