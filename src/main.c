#include "julo.h"
#include "cursor.h"
#include "gdt.h"
#include "idt.h"
#include "signal.h"
#include "keyboard.h"
#include "terminal.h"

signal_stack_t  signals = {
    .head =  0,
    .tail = 0,
};

int kernel_main(void) {
	init_gdt();
	init_idt();
	init_cursor();
	init_terminal();
	init_signal();
	init_keyboard();

	while (true) {
	    while (signals.head > signals.tail) {
			signals.head--;
			if (signals.queue[signals.head].int_nb < 32)
			    handle_exceptions((int_regs_t *)signals.queue[signals.head].data);
			else if  (signals.queue[signals.head].int_nb >= 32
			    && signals.queue[signals.head].int_nb <= 47)
			    handle_irq((int_regs_t *)signals.queue[signals.head].data);
            ft_bzero(&signals.queue[signals.head], sizeof(*(signals.queue)));
		}
	}
	return (0);
}
