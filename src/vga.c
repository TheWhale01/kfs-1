#include "julo.h"

static inline void vga_entry(const terminal_t *terminal, unsigned char c) {
    terminal->VGA_MEMORY[terminal->cursor->y * VGA_WIDTH + terminal->cursor->x]
        = (uint16_t)c | (uint16_t)(terminal->fcolor | terminal->bcolor << 4) << 8;
}

static inline unsigned char hex_digit(int v) {
    if (v >= 0 && v < 10)
        return v + '0';
    return ('a' + v - 10);
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

size_t printaddr(terminal_t *terminal, const void *p) {
    int i = 0;
    size_t len = 0;
    uintptr_t ptr_v = (uintptr_t)p;

    len += vga_putstring(terminal, "0x");
    if (!p) {
        vga_putchar(terminal, '0');
        return ++len;
    }
    i = (sizeof(ptr_v) << 3) - 4;
    while (hex_digit((ptr_v >> i) & 0xf) == '0' && i >= 0)
        i -= 4;
    len += i;
    while (i >= 0) {
        vga_putchar(terminal, hex_digit((ptr_v >> i) & 0xf));
        i -= 4;
    }
    return len;
}

size_t putnbr_base(terminal_t* terminal, int nb, const char *base) {
    size_t len = 0;
    size_t base_len = strlen(base);
    size_t nbr;

    base_len = strlen(base);
    if (nb < 0)  {
        nbr = nb * -1;
        len++;
        vga_putchar(terminal, '-');
    }
    else
        nbr = nb;
    if (nbr / base_len)
        len += putnbr_base(terminal, nbr / base_len, base);
    len++;
    vga_putchar(terminal, base[nbr % base_len]);
    return len;
}

size_t putnbr_base_u(terminal_t *terminal, size_t nb, const char *base) {
    size_t len = 0;
    size_t base_len = strlen(base);

    if (nb / base_len)
        len = putnbr_base(terminal, nb / base_len, base);
    vga_putchar(terminal, base[nb % base_len]);
    return ++len;
}
