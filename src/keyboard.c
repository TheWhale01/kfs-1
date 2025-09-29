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
    uint8_t scancode = inb(0x60) & 0x7F;
    uint8_t pressed = inb(0x60) & 0x80;

    printf("Scancode: %d\nPressed: %d\n", scancode, pressed);
}
