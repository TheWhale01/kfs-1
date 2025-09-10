#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>

#define VGA_HEIGHT 25
#define VGA_WIDTH 80

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

typedef enum {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
} vga_color_e;

typedef struct {
    size_t x;
    size_t y;
    const uint8_t end;
    const uint8_t start;
    volatile const size_t addr1;
    volatile const size_t addr2;
} cursor_t;

typedef struct {
    cursor_t *cursor;
    uint16_t fcolor;
    uint16_t bcolor;
    volatile uint16_t* const VGA_MEMORY;
} terminal_t;

void scroll(cursor_t *cursor);
void line_break(cursor_t *cursor);
void outb(uint16_t port, uint8_t val);
void update_cursor(const cursor_t *cursor);
void enable_cursor(const cursor_t *cursor);
void disable_cursor(const cursor_t *cursor);
void vga_putchar(terminal_t *terminal, char c);

size_t printaddr(terminal_t *terminal, const void *p);
size_t printk(terminal_t *terminal, const char *s, ...);
size_t vga_putstring(terminal_t *terminal, const char *str);
size_t putnbr_base(terminal_t* terminal, int nb, const char *base);
size_t putnbr_base_u(terminal_t* terminal, size_t nb, const char *base);

uint8_t inb(uint16_t port);
