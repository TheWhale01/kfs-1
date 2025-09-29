#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include "libft.h"

#define KERN_EMERG "0"
#define KERN_ALERT "1"
#define KERN_CRIT "2"
#define KERN_ERR "3"
#define KERN_WARNING "4"
#define KERN_NOTICE "5"
#define KERN_INFO "6"
#define KERN_DEBUG "7"

#define DEC_BASE "0123456789"
#define HEX_BASE "0123456789abcdef"
#define HEX_BASE_UPPER "0123456789ABCDEF"

#define SET_FLAG(x, flag) x |= (flag)
#define UNSET_FLAG(x, flag) x &= ~(flag)

void scroll(void);
void line_break(void);
void enable_cursor(void);
void update_cursor(void);
void vga_putchar(char c);
void line_backspace(void);
void disable_cursor(void);
void line_tabulation(void);
void line_vertical_tab(void);
void line_carriage_return(void);
void outb(uint16_t port, uint8_t val);
void change_screen(size_t new_screen);

size_t printaddr(const void *p);
size_t printk(const char *s, ...);
size_t printf(const char *fmt, ...);
size_t vga_putstring(const char *str);
size_t putnbr_base(int nb, const char *base);
size_t putnbr_base_u(size_t nb, const char *base);

uint8_t inb(uint16_t port);

extern void reboot(void);
