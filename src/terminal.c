#include "cursor.h"
#include "terminal.h"
#include "julo.h"
#include "libft.h"

terminal_t terminal = {
	.screen = 0,
   .fcolor = VGA_COLOR_WHITE,
   .bcolor = VGA_COLOR_BLACK,
   .VGA_MEMORY = (volatile uint16_t* const)0xB8000,
};

inline void vga_entry(unsigned char c) {
	terminal.VGA_MEMORY[cursor.VGA_Y[terminal.screen] * VGA_WIDTH + cursor.VGA_X[terminal.screen]]
        = (uint16_t)c | (uint16_t)(terminal.fcolor | terminal.bcolor << 4) << 8;
}

bool check_echappement(char c) {
	switch (c)
	{
		case '\n':
		    line_break();
		    handle_cmd();
			return (true);
		case '\a':
			return (true);
		case '\b': {
    			size_t len = ft_strlen(terminal.CMD_BUFFER[terminal.screen]);
    			terminal.CMD_BUFFER[terminal.screen][len - 1] = '\0';
    		}
			line_backspace();
			return (true);
		case '\t':
			line_tabulation();
			return (true);
		case '\r':
			line_carriage_return();
			return (true);
		case '\v':
		case '\f':
			line_vertical_tab();
			return (true);
		default:
			return (false);
	}
}

void vga_putchar(char c) {
    if (check_echappement(c)) {
        return ;
    }
   	vga_entry(c);
	if (++cursor.VGA_X[terminal.screen] >= VGA_WIDTH) {
		cursor.VGA_X[terminal.screen] = 0;
		if (cursor.VGA_Y[terminal.screen] == VGA_HEIGHT - 1)
			scroll();
		else
			cursor.VGA_Y[terminal.screen]++;
	}
	update_cursor();
}

size_t vga_putstring(const char *str) {
	size_t len = 0;

	while (*str) {
		vga_putchar(*str++);
		len++;
	}
	return len;
}

void scroll(void) {
	size_t	i = -1;
	while (++i < VGA_WIDTH * VGA_HEIGHT - VGA_WIDTH)
		terminal.VGA_MEMORY[i] = terminal.VGA_MEMORY[i + VGA_WIDTH];
	while (i < VGA_WIDTH * VGA_HEIGHT) {
		terminal.VGA_MEMORY[i] = (uint16_t)' ' | (uint16_t)(terminal.fcolor | terminal.bcolor << 4) << 8;
		i++;
	}
	cursor.VGA_Y[terminal.screen] = VGA_HEIGHT - 1;
}

void change_screen(size_t new_screen) {
	if (new_screen == terminal.screen)
		return ;
	ft_memcpy(terminal.VGA_SCREEN[terminal.screen],
		(const char *)terminal.VGA_MEMORY, VGA_WIDTH * VGA_HEIGHT);
	terminal.screen = new_screen;
	for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
		terminal.VGA_MEMORY[i] = terminal.VGA_SCREEN[new_screen][i];
	update_cursor();
}

void init_terminal(void) {
	for (size_t i = 0; i < NB_SCREEN; i++) {
		cursor.VGA_X[i] = 0, cursor.VGA_Y[i] = 0;
		ft_bzero(terminal.CMD_BUFFER[i], VGA_WIDTH);
		for (size_t j = 0; j < VGA_WIDTH * VGA_HEIGHT; j++)
			terminal.VGA_SCREEN[i][j] = (uint16_t)' ' | (uint16_t)(terminal.fcolor | terminal.bcolor << 4) << 8;
	}
	for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
		terminal.VGA_MEMORY[i] = (uint16_t)' ' | (uint16_t)(terminal.fcolor | terminal.bcolor << 4) << 8;
}

void clearscr() {
	cursor.VGA_X[terminal.screen] = 0;
	cursor.VGA_Y[terminal.screen] = 0;
	update_cursor();
	for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
		terminal.VGA_MEMORY[i] = (uint16_t)' ' | (uint16_t)(terminal.fcolor | terminal.bcolor << 4) << 8;
	ft_memcpy(terminal.VGA_SCREEN[terminal.screen], (const char *)terminal.VGA_MEMORY, VGA_WIDTH * VGA_HEIGHT);
}
