#include "keyboard.h"
#include "idt.h"
#include "julo.h"
#include <stdint.h>

void init_keyboard(void) {
    // Register keyboard PS/2 driver
    irq_install_handler(1, &keyboard_handler);
}

void keyboard_handler(int_regs_t *regs) {
	(void)regs;

	static const char SC_US[128]    = SCANCODE_INIT;
	static const char SC_SHIFT[128] = SCANCODE_SHIFT_INIT;
	static bool shift_on = false;
	uint8_t scancode = inb(0x60) & 0x7F;
	bool pressed = !(inb(0x60) & 0x80);
	if (scancode == SC_LSHIFT || scancode == SC_RSHIFT)
		shift_on = pressed;
	if (pressed)
	{
		if (!check_echappement(SC_US[scancode]))
			printf("%c", (shift_on ? SC_SHIFT[scancode] : SC_US[scancode]));
	}
	// else
	// 	printf("\nrelachement\n");
}
