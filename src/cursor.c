#include "julo.h"

void line_break(terminal_t *terminal) {
	terminal->cursor->x = 0;
	terminal->cursor->y++;
	if (terminal->cursor->y == VGA_HEIGHT)
	scroll(terminal);
	update_cursor(terminal->cursor);
}

void line_tabulation(terminal_t *terminal) {
	terminal->cursor->x += 4;
	if (terminal->cursor->x >= VGA_WIDTH - 4) {
		terminal->cursor->x = 4;
		terminal->cursor->y++;
	}
	if (terminal->cursor->y == VGA_HEIGHT)
		scroll(terminal);
	update_cursor(terminal->cursor);
}

void line_vertical_tab(terminal_t *terminal) {
	terminal->cursor->y++;
	if (terminal->cursor->y == VGA_HEIGHT)
		scroll(terminal);
	update_cursor(terminal->cursor);
}

void line_backspace(cursor_t *cursor) {
	if (cursor->x == 0 && cursor->y == 0)
		return ;
	cursor->x--;
	if (cursor->x == 0) {
		cursor->x = VGA_WIDTH - 1;
		cursor->y--;
	}
	update_cursor(cursor);
}

void line_carriage_return(cursor_t *cursor) {
	cursor->x = 0;
	update_cursor(cursor);
}

void enable_cursor(const cursor_t *cursor) {
	outb(cursor->addr1, 0x0A);
	outb(cursor->addr2, (inb(cursor->addr2) & 0xC0) | cursor->start);
	outb(cursor->addr1, 0x0B);
	outb(cursor->addr2, (inb(cursor->addr2) & 0xE0) | cursor->end);
}

void disable_cursor(const cursor_t *cursor) {
	outb(cursor->addr1, 0x0A);
	outb(cursor->addr2, 0x20);
}

void update_cursor(const cursor_t *cursor) {
	uint16_t pos = cursor->y * VGA_WIDTH + cursor->x;

	outb(cursor->addr1, 0x0F);
	outb(cursor->addr2, (uint8_t)(pos & 0xFF));
	outb(cursor->addr1, 0x0E);
	outb(cursor->addr2, (uint8_t)((pos >> 8) & 0xFF));
}