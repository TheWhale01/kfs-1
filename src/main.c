#include "julo.h"
#include "libft.h"
#include "cursor.h"
#include "gdt.h"
#include "idt.h"
#include "signal.h"
#include "keyboard.h"
#include "terminal.h"
#include "timer.h"

signal_stack_t  signals = {
    .size =  0,
    .queue = {0},
};

int kernel_main(void) {
	init_gdt();
	init_idt();
	init_timer();
	init_cursor();
	init_terminal();
	init_signal();
	init_keyboard();

	int q;
	int a = 1;
	int d = 0;
        asm volatile(
            "xorl %%edx, %%edx\n\t"   /* EDX = 0 (haut du dividende) */
            "movl %1, %%eax\n\t"      /* placer dividende en EAX */
            "divl %2\n\t"             /* divise EDX:EAX par %2 -> diviseur 0 provoque exception */
            : "=a"(q)                 /* sortie : quotient dans EAX */
            : "r"(a), "r"(d)          /* entrées */
        );
	while (true) {
	    while (signals.size > 0) {
			signals.size--;
			if  (signals.queue[signals.size].int_nb >= 32
			    && signals.queue[signals.size].int_nb <= 47)
			    handle_irq((int_regs_t *)signals.queue[signals.size].data);
            ft_bzero(&signals.queue[signals.size], sizeof(*(signals.queue)));
		}
	}
	return (0);
}
