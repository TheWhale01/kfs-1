#include "kfs.h"

void terminal_initialize(term_data *t_data)
{
	t_data->cursor->pos_y = 0;
	t_data->cursor->pos_x = 0;
	t_data->color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
	t_data->buffer = (uint16_t*) 0xB8000; // Buffer for screen
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			t_data->buffer[index] = vga_entry(' ', t_data->color);
		}
	}
}

void terminal_setcolor(term_data *t_data, uint8_t color)
{
	t_data->color = color;
}

void terminal_putentryat(char c, term_data *t_data)
{
	const size_t index = t_data->cursor->pos_y * VGA_WIDTH + t_data->cursor->pos_x;
	t_data->buffer[index] = vga_entry(c, t_data->color);
}

void terminal_putchar(char c, term_data *t_data)
{
	switch (c) {
		case '\n':
			t_data->cursor->pos_x = 0;
			++t_data->cursor->pos_y;
			break;
		default:
			terminal_putentryat(c, t_data);
			if (++t_data->cursor->pos_x == VGA_WIDTH) {
				t_data->cursor->pos_x = 0;
				if (++t_data->cursor->pos_y == VGA_HEIGHT)
					t_data->cursor->pos_y = 0;
			}
			break;
	}
	cursor_update(t_data->cursor);
}

void terminal_writestr(const char* str, size_t size, term_data *t_data)
{
	for (size_t i = 0; i < size; i++)
		terminal_putchar(str[i], t_data);
}

inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
	return fg | bg << 4;
}

inline uint16_t vga_entry(unsigned char uc, uint8_t color)
{
	return (uint16_t) uc | (uint16_t) color << 8;
}


