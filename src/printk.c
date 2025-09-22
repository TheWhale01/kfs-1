#include "julo.h"

static inline unsigned char hex_digit(int v) {
    if (v >= 0 && v < 10)
        return v + '0';
    return ('a' + v - 10);
}

size_t printaddr(const void *p) {
    int i = 0;
    size_t len = 0;
    uintptr_t ptr_v = (uintptr_t)p;

    len += vga_putstring("0x");
    if (!p) {
        vga_putchar('0');
        return ++len;
    }
    i = (sizeof(ptr_v) << 3) - 4;
    while (hex_digit((ptr_v >> i) & 0xf) == '0' && i >= 0)
        i -= 4;
    len += i;
    while (i >= 0) {
        vga_putchar(hex_digit((ptr_v >> i) & 0xf));
        i -= 4;
    }
    return len;
}

size_t putnbr_base(int nb, const char *base) {
    size_t len = 0;
    size_t base_len = ft_strlen(base);
    size_t nbr;

    base_len = ft_strlen(base);
    if (nb < 0)  {
        nbr = nb * -1;
        len++;
        vga_putchar('-');
    }
    else
        nbr = nb;
    if (nbr / base_len)
        len += putnbr_base(nbr / base_len, base);
    len++;
    vga_putchar(base[nbr % base_len]);
    return len;
}

size_t putnbr_base_u(size_t nb, const char *base) {
    size_t len = 0;
    size_t base_len = ft_strlen(base);

    if (nb / base_len)
        len = putnbr_base(nb / base_len, base);
    vga_putchar(base[nb % base_len]);
    return ++len;
}

static size_t print_log_level(char c) {
    static const char *priority_msg[] = {
        "EMERGENCY: ",
        "ALERT: ",
        "CRITICAL: ",
        "ERROR: ",
        "WARNING: ",
        "NOTICE: ",
        "INFO: ",
        "DEBUG: ",
    };
    static const vga_color_e priority_color[] = {
        VGA_COLOR_RED,
        VGA_COLOR_LIGHT_RED,
        VGA_COLOR_MAGENTA,
        VGA_COLOR_LIGHT_MAGENTA,
        VGA_COLOR_BROWN,
        VGA_COLOR_LIGHT_BROWN,
        VGA_COLOR_LIGHT_GREEN,
        VGA_COLOR_LIGHT_BLUE,
    };

    if (!(c >= '0' && c <= '7')) {
        terminal.fcolor = VGA_COLOR_WHITE;
        return 0;
    }
    c -= '0';
    terminal.fcolor = priority_color[(size_t)c];
    vga_putstring(priority_msg[(size_t)c]);
    return 1;
}

static size_t print_arg(const char *s, va_list args) {
    size_t len = 0;

    switch (*s) {
        case 'c':
            vga_putchar((char)va_arg(args, int));
            len++;
            break;
        case 's':
            len += vga_putstring(va_arg(args, char*));
            break;
        case 'p':
            len += printaddr(va_arg(args, void*));
            break;
        case 'd':
            len += putnbr_base(va_arg(args, int), DEC_BASE);
            break;
        case 'i':
            len += putnbr_base(va_arg(args, int), DEC_BASE);
            break;
        case 'u':
            len += putnbr_base_u(va_arg(args, size_t), DEC_BASE);
            break;
        case 'x':
            len += putnbr_base(va_arg(args, size_t), HEX_BASE);
            break;
        case 'X':
            len += putnbr_base(va_arg(args, size_t), HEX_BASE_UPPER);
            break;
        case '%':
            len++;
            vga_putchar('%');
            break;
        default:
            break;
    }
    return len;
}

size_t printk(const char *s, ...) {
    size_t len = 0;
    va_list args;

    va_start(args, s);
    len += print_log_level(*s);
    s += len;
    while (*s) {
        while (*s == '%') {
            len += print_arg(++s, args);
            s++;
        }
        if (!*s)
            break;
        vga_putchar(*s);
        len++;
        s++;
    }
    va_end(args);
    terminal.fcolor = VGA_COLOR_WHITE;
    return len;
}
