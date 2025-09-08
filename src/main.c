#include "julo.h"

int kernel_main(void) {
    cursor_t cursor = {.x = 0, .y = 0 };
    terminal_t terminal = {
        .cursor = &cursor,
        .fcolor = VGA_COLOR_WHITE,
        .bcolor = VGA_COLOR_BLACK,
        .VGA_MEMORY = (volatile uint16_t*)0xB8000,
    };

    printk(&terminal, KERN_EMERG "%p", &cursor);
    return (0);
}
