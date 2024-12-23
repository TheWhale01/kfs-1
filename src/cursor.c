#include "kfs.h"

void cursor_enable(cursor_data *c_data) {
	c_data->width = 0;
	c_data->height = 15;
	c_data->pos_x = 0;
	c_data->pos_y = 0;
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | c_data->width);

	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | c_data->height);
}

void cursor_disable(void) {
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}

void cursor_update(cursor_data *c_data) {
	uint16_t pos = c_data->pos_y * VGA_WIDTH + c_data->pos_x;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t)(pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}
