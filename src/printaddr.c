#include "julo.h"

static inline unsigned char hex_digit(int v) {
    if (v >= 0 && v < 10)
        return v + '0';
    return ('a' + v - 10);
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
