#include "keyboard.h"
#include "idt.h"
#include "julo.h"
#include <stdint.h>

void init_keyboard(void) {
    // Register keyboard PS/2 driver
    irq_install_handler(1, &keyboard_handler);
}

void keyboard_handler(int_regs_t *regs) {
    static volatile bool pressed = true;
    uint8_t code = inb(0x60);
}
