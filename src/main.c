#include "julo.h"
#include "cursor.h"
#include "gdt.h"
#include "idt.h"
#include "keyboard.h"
#include "terminal.h"

int kernel_main(void) {
	init_gdt();
	init_idt();
	init_cursor();
	init_terminal();
	init_keyboard();
	return (0);
}
