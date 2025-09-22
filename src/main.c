#include "julo.h"

void make_pattern_2000(char *out)
{
	for (size_t y = 0; y < VGA_HEIGHT; ++y)
	{
		char ch = (char)('A' + (y % (VGA_WIDTH + 1)));
		for (size_t x = 0; x < VGA_WIDTH; ++x)
			out[y*VGA_WIDTH + x] = ch;
	}
	out[VGA_HEIGHT * VGA_WIDTH] = '\0';
}

void initTerminal(terminal_t *terminal) {
    for (size_t i = 0; i < NB_SCREEN; i++)
		terminal->cursor->VGA_X[i] = 0, terminal->cursor->VGA_Y[i] = 0;
	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < VGA_WIDTH * VGA_HEIGHT; j++)
			terminal->VGA_SCREEN[i][j] = (uint16_t)' ' | (uint16_t)(terminal->fcolor | terminal->bcolor << 4) << 8;
	}
	for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
		terminal->VGA_MEMORY[i] = (uint16_t)' ' | (uint16_t)(terminal->fcolor | terminal->bcolor << 4) << 8;
	enable_cursor(terminal->cursor);
}

int kernel_main(void) {
    char s2000[VGA_WIDTH * VGA_HEIGHT + 1];
    cursor_t cursor = {
		.x = 0, .y = 0,
        .start = 15, .end = 15,
        .addr1 = 0x3D4, .addr2 = 0x3D5,
    };
    terminal_t terminal = {
        .cursor = &cursor,
		.screen = 0,
        .fcolor = VGA_COLOR_WHITE,
        .bcolor = VGA_COLOR_BLACK,
        .VGA_MEMORY = (volatile uint16_t* const)0xB8000,
	};

	// term 0
	initTerminal(&terminal);
	// printk(&terminal, str);

	// term 1
	change_screen(&terminal, 1);
	for (size_t i = 0; i < VGA_WIDTH; i++)
        vga_putchar(&terminal, 'A');
	make_pattern_2000(s2000);
	vga_putstring(&terminal, s2000);
	vga_putstring(&terminal, "i\brrrrrrrrrrrrrrrrrrrrrr\trrrrrrrrrrrrrerr\vrrrrrrrrrrss\fsssssssssss\asssssssssssssssssssse\nssse\rsex");

	// term 2
	change_screen(&terminal, 2);
	vga_putstring(&terminal, "ssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss\t");

	// term 0
	change_screen(&terminal, 0);
	for (int i = 0; i < 80; i++)
		vga_putstring(&terminal, "1");
	vga_putstring(&terminal, "\r");
	for (int i = 0; i < 80; i++)
	    vga_putstring(&terminal, "2");

	for (int i = 0; i < 160; i++)
		vga_putstring(&terminal, "2");
    printk(&terminal, KERN_EMERG "%p\n", &cursor);
    printk(&terminal, KERN_ALERT "%d\n", 13);
    printk(&terminal, KERN_CRIT "%i\n", 12356543);
    printk(&terminal, KERN_ERR "%u\n", 4294967295);
    printk(&terminal, KERN_WARNING "%c\n", 'G');
    printk(&terminal, KERN_NOTICE "%s\n", "BONSOIR JE NE SUIS PAS MOI");
    printk(&terminal, KERN_INFO "%x\n", 15);
    printk(&terminal, KERN_DEBUG "%X\n", 15);
    printk(&terminal, "IMPOSTEUR: %%");
    return (0);
}
