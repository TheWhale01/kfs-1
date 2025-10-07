#include "julo.h"
#include "libft.h"
#include "cursor.h"
#include "gdt.h"
#include "idt.h"
#include "signal.h"
#include "keyboard.h"
#include "terminal.h"
#include "timer.h"
#include "syscalls.h"
#include "tests.h"

signal_stack_t	signals = {
    .size =  0,
};

int kernel_main(void) {
	init_gdt();
	init_idt();
	init_timer();
	init_syscalls();
	init_cursor();
	init_terminal();
	init_signal();
	init_keyboard();

	// test_div_0();
	test_sys_write();
	while (true) {
	    while (signals.size > 0) {
			signals.size--;
			if  (signals.queue[signals.size].int_nb >= 32
				&& signals.queue[signals.size].int_nb <= 47)
				handle_irq((int_regs_t *)signals.queue[signals.size].data);
			else if (signals.queue[signals.size].int_nb == 0x80)
    			printf("RET: %d\n", handle_syscall((int_regs_t *)signals.queue[signals.size].data));
			ft_bzero(&signals.queue[signals.size], sizeof(*(signals.queue)));
		}
	}
	return (0);
}
