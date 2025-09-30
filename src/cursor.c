#include "julo.h"
#include "terminal.h"
#include "cursor.h"

cursor_t cursor = {
   .start = 15, .end = 15,
   .addr1 = 0x3D4, .addr2 = 0x3D5,
};

static int check_last_enter(void) {
    int x = VGA_WIDTH;

    while (--x >= 0) {
        if ((terminal.VGA_MEMORY[cursor.VGA_Y[terminal.screen] * VGA_WIDTH + x] & 0x00FF) != ' ')
            return x;
    }
    return 0;
}

void line_break(void) {
	cursor.VGA_X[terminal.screen] = 0;
	cursor.VGA_Y[terminal.screen]++;
	if (cursor.VGA_Y[terminal.screen] == VGA_HEIGHT)
    	scroll();
	update_cursor();
}

void line_tabulation(void) {
	cursor.VGA_X[terminal.screen] += 4;
	if (cursor.VGA_X[terminal.screen] >= VGA_WIDTH - 4) {
		cursor.VGA_X[terminal.screen] = 4;
		cursor.VGA_Y[terminal.screen]++;
	}
	if (cursor.VGA_Y[terminal.screen] == VGA_HEIGHT)
		scroll();
	update_cursor();
}

void line_vertical_tab(void) {
	cursor.VGA_Y[terminal.screen]++;
	if (cursor.VGA_Y[terminal.screen] == VGA_HEIGHT)
		scroll();
	update_cursor();
}

void line_backspace(void) {
	if (cursor.VGA_X[terminal.screen] == 0 && cursor.VGA_Y[terminal.screen] == 0)
		return ;
	if (cursor.VGA_X[terminal.screen] != 0)
		cursor.VGA_X[terminal.screen]--;
	else
	{
	    cursor.VGA_Y[terminal.screen]--;
		cursor.VGA_X[terminal.screen] = check_last_enter();
	}
	update_cursor();
	vga_entry(' ');
}

void line_carriage_return(void) {
	cursor.VGA_X[terminal.screen] = 0;
	update_cursor();
}

void enable_cursor(void) {
	outb(cursor.addr1, 0x0A);
	outb(cursor.addr2, (inb(cursor.addr2) & 0xC0) | cursor.start);
	outb(cursor.addr1, 0x0B);
	outb(cursor.addr2, (inb(cursor.addr2) & 0xE0) | cursor.end);
}

void disable_cursor(void) {
	outb(cursor.addr1, 0x0A);
	outb(cursor.addr2, 0x20);
}

void update_cursor(void) {
	uint16_t pos = cursor.VGA_Y[terminal.screen] * VGA_WIDTH + cursor.VGA_X[terminal.screen];

	outb(cursor.addr1, 0x0F);
	outb(cursor.addr2, (uint8_t)(pos & 0xFF));
	outb(cursor.addr1, 0x0E);
	outb(cursor.addr2, (uint8_t)((pos >> 8) & 0xFF));
}

void init_cursor(void) {
    ft_bzero(cursor.VGA_X, NB_SCREEN);
    ft_bzero(cursor.VGA_Y, NB_SCREEN);
    enable_cursor();
}
