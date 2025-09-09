#include "julo.h"

static inline void vga_entry(const terminal_t *terminal, unsigned char c) {
    terminal->VGA_MEMORY[terminal->cursor->y * VGA_WIDTH + terminal->cursor->x]
        = (uint16_t)c | (uint16_t)(terminal->fcolor | terminal->bcolor << 4) << 8;
}

void vga_putchar(terminal_t *terminal, char c) {
    switch (c) {
        case '\n':
            line_break(terminal->cursor);
            return;
        default:
            break;
    }
    vga_entry(terminal, c);
    if (++(terminal->cursor->x) >= VGA_WIDTH) {
        terminal->cursor->x = 0;
        if (++(terminal->cursor->y) >= VGA_HEIGHT)
            terminal->cursor->y = 0;
    }
}

size_t vga_putstring(terminal_t *terminal, const char *str) {
    size_t len = 0;

    while (*str) {
        vga_putchar(terminal, *str++);
        len++;
    }
    return len;
}
