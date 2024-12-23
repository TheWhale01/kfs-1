#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

typedef struct {
	uint8_t width;
	uint8_t height;
	int pos_x;
	int pos_y;
} cursor_data;

typedef struct  {
	uint8_t color;
	uint16_t* buffer;
	cursor_data *cursor;
} term_data;


enum vga_color {
	VGA_COLOR_BLACK,
	VGA_COLOR_BLUE,
	VGA_COLOR_GREEN,
	VGA_COLOR_CYAN,
	VGA_COLOR_RED,
	VGA_COLOR_MAGENTA,
	VGA_COLOR_BROWN,
	VGA_COLOR_LIGHT_GREY,
	VGA_COLOR_DARK_GREY,
	VGA_COLOR_LIGHT_BLUE,
	VGA_COLOR_LIGHT_GREEN,
	VGA_COLOR_LIGHT_CYAN,
	VGA_COLOR_LIGHT_RED,
	VGA_COLOR_LIGHT_MAGENTA,
	VGA_COLOR_LIGHT_BROWN,
	VGA_COLOR_WHITE,
};

void			terminal_initialize(term_data *t_data);
void			terminal_setcolor(term_data *data, uint8_t color);
void			terminal_putentryat(char c, term_data *t_data);
void			terminal_putchar(char c, term_data *t_data);
void			terminal_writestr(const char *data, size_t size, term_data *t_data);

uint8_t			vga_entry_color(enum vga_color fs, enum vga_color bg);
uint16_t		vga_entry(unsigned char uc, uint8_t color);

void			cursor_enable(cursor_data *c_data);
void			cursor_disable(void);
void			cursor_update(cursor_data *c_data);

size_t			strlen(const char *str);
uint8_t			inb(uint16_t port);
void			outb(uint16_t port, uint8_t value);
