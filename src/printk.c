#include "julo.h"

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
        VGA_COLOR_CYAN,
        VGA_COLOR_LIGHT_CYAN,
        VGA_COLOR_GREEN,
        VGA_COLOR_LIGHT_GREEN,
    };
    if (!(c >= '0' && c <= '7'))
        return 0;
    c -= 48;
    terminal->fcolor = priority_color[(size_t)c];
    return vga_putstring(terminal, priority_msg[(size_t)c]);
}

static void print_arg(terminal_t *terminal, const char *s, va_list args, size_t *len) {
    switch (*s) {
        case 'c':
            vga_putchar(terminal, (char)va_arg(args, int));
            (*len)++;
            break;
        case 's':
            len += vga_putstring(terminal, va_arg(args, char* ));
            break;
        case 'p':
            len += vga_putnbr_base(terminal, va_arg(args, uintptr_t), HEX_BASE);
        default:
            break;
    }
}

size_t printk(terminal_t *terminal, const char *s, ...) {
    size_t len = 0;
    va_list args;

    va_start(args, s);
    len += print_log_level(terminal, *s);
    while (*s) {
        while (*s == '%') {
            print_arg(terminal, ++s, args, &len);
            s++;
        }
        if (!*s)
            break;
        vga_putchar(terminal, *s);
        len++;
    }
    return len;
}
