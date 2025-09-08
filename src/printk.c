#include "julo.h"
#include <stddef.h>

static size_t print_log_level(terminal_t *terminal, char c) {
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
        terminal->fcolor = VGA_COLOR_WHITE;
        return 0;
    }
    c -= '0';
    terminal->fcolor = priority_color[(size_t)c];
    vga_putstring(terminal, priority_msg[(size_t)c]);
    return 1;
}

static size_t print_arg(terminal_t *terminal, const char *s, va_list args) {
    size_t len = 0;

    switch (*s) {
        case 'c':
            vga_putchar(terminal, (char)va_arg(args, int));
            len++;
            break;
        case 's':
            len += vga_putstring(terminal, va_arg(args, char*));
            break;
        case 'p':
            len += printaddr(terminal, va_arg(args, void*));
            break;
        case 'd':
            len += putnbr_base(terminal, va_arg(args, int), DEC_BASE);
            break;
        case 'i':
            len += putnbr_base(terminal, va_arg(args, int), DEC_BASE);
            break;
        case 'u':
            len += putnbr_base_u(terminal, va_arg(args, size_t), DEC_BASE);
            break;
        case 'x':
            len += putnbr_base(terminal, va_arg(args, size_t), HEX_BASE);
            break;
        case 'X':
            len += putnbr_base(terminal, va_arg(args, size_t), HEX_BASE_UPPER);
            break;
        case '%':
            len++;
            vga_putchar(terminal, '%');
            break;
        default:
            break;
    }
    return len;
}

size_t printk(terminal_t *terminal, const char *s, ...) {
    size_t len = 0;
    va_list args;

    va_start(args, s);
    len += print_log_level(terminal, *s);
    s += len;
    while (*s) {
        while (*s == '%') {
            len += print_arg(terminal, ++s, args);
            s++;
        }
        if (!*s)
            break;
        vga_putchar(terminal, *s);
        len++;
        s++;
    }
    va_end(args);
    return len;
}
