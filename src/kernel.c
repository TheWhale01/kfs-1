#include "kfs.h"

void print_arg(const char *str, va_list args, int *len, term_data *t_data) {
	switch (*str) {
		case 'c':
			terminal_putchar(va_arg(args, int), t_data);
			(*len)++;
			break;
		case 's': {
			char *s = va_arg(args, char *);
			terminal_writestr(s, strlen(s), t_data);
			(*len)++;
			break;
		}
		default:
			break;
	}
}

int printf(term_data *t_data, const char *str, ...) {
	int len = 0;
	va_list args;

	va_start(args, str);
	while (*str) {
		while (*str == '%') {
			print_arg(++str, args, &len, t_data);
			str++;
		}
		if (*str) {
			terminal_putchar(*(str++), t_data);
			len++;
		}
			
	}
	va_end(args);
	return len;
}

void kernel_main(void)
{
	term_data t_data;
	cursor_data c_data;
	t_data.cursor = &c_data;
	const char *welcome_str = "BonsoirOS\n";

	terminal_initialize(&t_data);
	cursor_enable(&c_data);
	terminal_writestr(welcome_str, strlen(welcome_str), &t_data);
}
