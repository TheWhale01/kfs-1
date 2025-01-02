#include "kfs.h"

void kernel_main(void)
{
	term_data t_data;
	cursor_data c_data;
	t_data.cursor = &c_data;
	const char *welcome_str = "42\nsalut !";

	terminal_initialize(&t_data);
	cursor_enable(&c_data);
	terminal_writestr(welcome_str, strlen(welcome_str), &t_data);
}
