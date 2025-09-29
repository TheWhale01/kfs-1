#include "keyboard.h"
#include "idt.h"
#include "julo.h"
#include <stdint.h>

static const char SC_US[128]    = SCANCODE_INIT;
static const char SC_SHIFT[128] = SCANCODE_SHIFT_INIT;
bool shift_on = false;

void init_keyboard(void) {
    // Register keyboard PS/2 driver
    irq_install_handler(1, &keyboard_handler);
}

void keyboard_handler(int_regs_t *regs) {
    (void)regs;
    uint8_t scancode = inb(0x60) & 0x7F;
    bool pressed = !(inb(0x60) & 0x80);
    if (scancode == SC_LSHIFT || scancode == SC_RSHIFT)
		shift_on = true;
	if (pressed)
    	printf("affichage: %c\n", (shift_on ? SC_SHIFT[scancode] : SC_US[scancode]));
	else
		printf("Scancode: 0x%X IL A RELACHE\n", scancode);
}
