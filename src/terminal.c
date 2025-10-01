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
void stack(int nb) {
	if (nb < 0 || nb > 16884 - 60)
		nb = 0;
	uint32_t *esp_snapshot;
	asm volatile ("mov %%esp, %0" : "=r"(esp_snapshot));

	printf("\n=== Kernel stack dump ===\n");
	printf("ESP snapshot = %p index : %d to %d\n\n", (void*)esp_snapshot, nb, nb + 60);
	for (int i = nb; i < nb + 10; ++i) {
		for (int j = 0; j < 3; ++j) {
			uint32_t *addr = esp_snapshot + i*3 + j;
			uint32_t  val  = *addr;
			printf("%p: %X       ", (void*)addr, val);
		}
		printf("\n");
	}
}

void debug(const char *buff) {(void)buff;}


void setcolor(const char *buff) {
	size_t arg1_len = ft_strnlen(buff, ' ');
	const char *colors[] = {"black", "blue", "green", "cyan", "red", "magenta",
		"brown", "light_grey", "dark_grey", "light_blue", "light_green", "light_cyan",
		"light_red", "light_magenta", "light_brown", "white"};

	for (size_t i = 0; i < 16; i++) {
		if (!ft_strncmp(colors[i], buff, ft_strlen(colors[i])) && ft_strlen(colors[i]) == arg1_len) {
			terminal.fcolor = i;
			break;
		}
	}
	for (size_t i = 0; i < 16; i++) {
		if (!ft_strncmp(colors[i], buff + arg1_len + 1, ft_strlen(colors[i])) && ft_strlen(colors[i]) == ft_strlen(buff + arg1_len + 1)) {
			terminal.bcolor = i;
			break;
		}
	}
}

void info() {
	printf("\nCommande      Syntaxe / Usage                         Description\n");
	printf("--------------------------------------------------------------------------\n");
	printf("screen        screen   <1..4>               Selectionne l'ecran 1 a 4\n");
	printf("clear         clear                         Efface l'ecran\n");
	printf("setcolor      setcolor <fcolor> <bcolor>    Change les couleurs texte/fond\n");
	printf("debug         debug    <priority> <msg>     Log de debug\n");
	printf("priority      priority                      Liste des priorites\n");
	printf("stack         stack    <index>              Affiche la stack courante\n");
	printf("reboot        reboot                        Redemarre la machine\n");
	printf("shutdown      shutdown                      Eteint la machine\n");
}

void priority()
{
	printf("Priority Messages List: \n[EMERGENCY] [ALERT] [CRITICAL] [ERROR] [WARNING] [NOTICE] [INFO] [DEBUG]\n");
}

void halt() {
	return;
}

void handle_cmd() {
	char buff[VGA_WIDTH];
	size_t word_len = ft_strnlen(terminal.CMD_BUFFER[terminal.screen], ' ');

	if (terminal.CMD_BUFFER[terminal.screen][0] == '\0')
		return ;
	ft_strlcpy(buff, terminal.CMD_BUFFER[terminal.screen], VGA_WIDTH);
	ft_bzero(terminal.CMD_BUFFER[terminal.screen], VGA_WIDTH);
	if (!ft_strncmp(buff, "info", ft_strlen("info")) && ft_strlen("info") == word_len)
		info();
	else if (!ft_strncmp(buff, "stack", ft_strlen("stack")) && ft_strlen("stack") == word_len)
		stack(ft_atoi(buff + word_len + 1));
	else if (!ft_strncmp(buff, "reboot", ft_strlen("reboot")) && ft_strlen("reboot") == word_len)
		reboot();
	else if (!ft_strncmp(buff, "shutdown", ft_strlen("shutdown")) && ft_strlen("shutdown") == word_len)
		halt();
	else if (!ft_strncmp(buff, "clear", ft_strlen("clear")) && ft_strlen("clear") == word_len)
		clearscr();
	else if (!ft_strncmp(buff, "debug", ft_strlen("debug")) && ft_strlen("debug") == word_len)
		debug(buff + word_len + 1);
	else if (!ft_strncmp(buff, "priority", ft_strlen("priority")) && ft_strlen("priority") == word_len)
		priority();
	else if (!ft_strncmp(buff, "setcolor", ft_strlen("setcolor")) && ft_strlen("setcolor") == word_len)
		setcolor(buff + word_len + 1);
	else if (!ft_strncmp(buff, "screen", ft_strlen("screen")) && ft_strlen("screen") == word_len)
		change_screen(ft_atoi(buff + word_len + 1) - 1);
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
	if (new_screen == terminal.screen || new_screen > 3)
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
