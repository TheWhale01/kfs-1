#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include "libft.h"

#define VGA_HEIGHT 25
#define VGA_WIDTH 80
#define NB_SCREEN 4

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

#define IDT_SIZE 256

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
	size_t VGA_X[NB_SCREEN];
	size_t VGA_Y[NB_SCREEN];
    const uint8_t end;
    const uint8_t start;
    volatile const size_t addr1;
    volatile const size_t addr2;
} cursor_t;

// https://wiki.osdev.org/Interrupt_Descriptor_Table
// __attribute__((packed)) means that the compiler will compile the structure as
// is in assembly without any sort of optimization. This will ensure the good
// functioning of the structure
typedef struct {
    uint16_t offset_1;
    uint16_t selector;
    uint8_t zero;
    uint8_t type_attributes;
    uint16_t offset_2;
} __attribute((packed)) idt_entry_t;

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_ptr_t;

typedef struct {
    idt_ptr_t *ptr;
    idt_entry_t *entries[IDT_SIZE];
} idt_t;

typedef struct {
    cursor_t *cursor;
	size_t screen;
    uint16_t fcolor;
    uint16_t bcolor;
    volatile uint16_t* const VGA_MEMORY;
	uint16_t VGA_SCREEN[NB_SCREEN][VGA_WIDTH * VGA_HEIGHT];
} terminal_t;

void initIDT(idt_t *idt);
void scroll(terminal_t *terminal);
void line_backspace(cursor_t *cursor);
void line_break(terminal_t *terminal);
void outb(uint16_t port, uint8_t val);
void line_tabulation(terminal_t *terminal);
void update_cursor(const cursor_t *cursor);
void enable_cursor(const cursor_t *cursor);
void line_carriage_return(cursor_t *cursor);
void disable_cursor(const cursor_t *cursor);
void line_vertical_tab(terminal_t *terminal);
void vga_putchar(terminal_t *terminal, char c);
void change_screen(terminal_t *terminal, size_t new_screen);
void setIDTGate(uint8_t num, uint32_t base, uint16_t selector, uint8_t flags);

size_t printaddr(terminal_t *terminal, const void *p);
size_t printk(terminal_t *terminal, const char *s, ...);
size_t vga_putstring(terminal_t *terminal, const char *str);
size_t putnbr_base(terminal_t* terminal, int nb, const char *base);
size_t putnbr_base_u(terminal_t* terminal, size_t nb, const char *base);

uint8_t inb(uint16_t port);
