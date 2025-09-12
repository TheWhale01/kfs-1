#include "julo.h"

static inline void vga_entry(const terminal_t *terminal, unsigned char c) {
	terminal->VGA_MEMORY[terminal->cursor->y * VGA_WIDTH + terminal->cursor->x]
        = (uint16_t)c | (uint16_t)(terminal->fcolor | terminal->bcolor << 4) << 8;
}

void vga_putchar(terminal_t *terminal, char c) {
	switch (c) {
		case '\n':
			line_break(terminal);
			return ;
		case '\a':
			return ;
		case '\b':
			line_backspace(terminal->cursor);
			vga_entry(terminal, ' ');
			return;
		case '\t':
			line_tabulation(terminal);
			return ;
		case '\r':
			line_carriage_return(terminal->cursor);
			return ;
		case '\v':
		case '\f':
			line_vertical_tab(terminal);
			return;
		default:
			break;
	}
	vga_entry(terminal, c);
	if (++(terminal->cursor->x) >= VGA_WIDTH) {
		terminal->cursor->x = 0;
		if (terminal->cursor->y == VGA_HEIGHT - 1)
			scroll(terminal);
		else
			terminal->cursor->y++;
	}
	update_cursor(terminal->cursor);
}

size_t vga_putstring(terminal_t *terminal, const char *str) {
	size_t len = 0;

	while (*str) {
		vga_putchar(terminal, *str++);
		len++;
	}
	return len;
}

void scroll(terminal_t *terminal) {
	size_t	i = -1;
	while (++i < VGA_WIDTH * VGA_HEIGHT - VGA_WIDTH)
		terminal->VGA_MEMORY[i] = terminal->VGA_MEMORY[i + VGA_WIDTH];
	while (i < VGA_WIDTH * VGA_HEIGHT) {
		terminal->VGA_MEMORY[i] = (uint16_t)' ' | (uint16_t)(terminal->fcolor | terminal->bcolor << 4) << 8;
		i++;
	}
	terminal->cursor->y = VGA_HEIGHT - 1;
}

void change_screen(terminal_t *terminal, size_t new_screen) {
	if (new_screen == terminal->screen)
		return ;
	terminal->cursor->VGA_X[terminal->screen] = terminal->cursor->x;
	terminal->cursor->VGA_Y[terminal->screen] = terminal->cursor->y;
	for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
		terminal->VGA_SCREEN[terminal->screen][i] = terminal->VGA_MEMORY[i];

	terminal->screen = new_screen;

	for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
		terminal->VGA_MEMORY[i] = terminal->VGA_SCREEN[new_screen][i];
	terminal->cursor->x = terminal->cursor->VGA_X[new_screen];
	terminal->cursor->y = terminal->cursor->VGA_Y[new_screen];
	update_cursor(terminal->cursor);
}
