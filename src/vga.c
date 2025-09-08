#include "julo.h"
#include <stdint.h>

static inline uint16_t vga_entry(unsigned char c, vga_color_e fcolor, vga_color_e bcolor) {
    return (uint16_t)c | ((uint16_t)(fcolor | bcolor << 4)) << 8;
}

void vga_putchar(terminal_t *terminal, char c) {
    if (terminal->cursor->x >= VGA_WIDTH)
        line_break(terminal->cursor);
    else if (terminal->cursor->y > VGA_HEIGHT)
        scroll(terminal->cursor);
    switch (c) {
        case '\n':
            line_break(terminal->cursor);
            return;
        default:
            break;
    }
    vga_entry(c, terminal->fcolor, terminal->bcolor);
    terminal->cursor->x++;
}

size_t vga_putstring(terminal_t *terminal, const char *str) {
    size_t len = 0;

    while (*str) {
        vga_putchar(terminal, *str++);
        len++;
    }
    return len;
}

size_t vga_putnbr_base(terminal_t *terminal, int nb, const char *base) {
    size_t nbr = 0;
    size_t len = 0;
    size_t base_len = 0;

    len = 0;
    base_len = strlen(base);
    if (nb < 0) {
        nbr = nb * -1;
        vga_putchar(terminal, '-');
        len++;
    }
    else
        nbr = nb;
    if (nbr / base_len)
        len += vga_putnbr_base(terminal, nbr, base);
    vga_putchar(terminal, base[nbr % base_len]);
    len++;
    return len;
}
