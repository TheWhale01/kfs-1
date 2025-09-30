#include "keyboard.h"
#include "idt.h"
#include "julo.h"
#include "terminal.h"

keyboard_t keyboard = {
    .SC_SHIFT = SCANCODE_SHIFT_INIT,
    .SC_US = SCANCODE_INIT,
    .shift_on = false, .alt_on = false,
    .scancode = 0,
    .pressed = false
};

void init_keyboard(void) {
    // Register keyboard PS/2 driver
    irq_install_handler(1, &keyboard_handler);
}

void keyboard_handler(int_regs_t *regs) {
	(void)regs;
	keyboard.scancode = inb(0x60) & 0x7F;
	keyboard.pressed = !(inb(0x60) & 0x80);
	switch (keyboard.scancode) {
	    case SC_LSHIFT:
		case SC_RSHIFT:
            keyboard.shift_on = keyboard.pressed;
            break;
        case SC_ALT:
            keyboard.alt_on = keyboard.pressed;
            break;
        default:
            break;
	}
	if (keyboard.pressed)
	{
	    if (keyboard.alt_on && (keyboard.scancode - 1 >= 1 && keyboard.scancode - 1 <= 4))
			change_screen(keyboard.scancode - 2);
		else if (!check_echappement(keyboard.SC_US[keyboard.scancode]) && !keyboard.alt_on)
		    vga_putchar(keyboard.shift_on ? keyboard.SC_SHIFT[keyboard.scancode] : keyboard.SC_US[keyboard.scancode]);
	}
}
