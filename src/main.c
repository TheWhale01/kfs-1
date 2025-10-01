#include "julo.h"
#include "cursor.h"
#include "gdt.h"
#include "idt.h"
#include "keyboard.h"
#include "terminal.h"

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

int kernel_main(void) {
	init_gdt();
	init_idt();
	init_terminal();
	init_cursor();
	init_keyboard();
    return (0);
}
