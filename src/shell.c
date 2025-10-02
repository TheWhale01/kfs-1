#include "julo.h"
#include "libft.h"
#include "terminal.h"
#include <stdint.h>

void stack(int nb) {
	if (nb < 0 || nb > 16884 - 60)
		nb = 0;
	uint32_t *esp_snapshot;
	asm volatile ("mov %%esp, %0" : "=r"(esp_snapshot));

	printf("\n=== Kernel stack dump ===\n");
	printf("ESP snapshot = %p index : %d to %d\n", (void*)esp_snapshot, nb, nb + 60);
	printf("Address   Value            Address   Value            Address   Value\n\n");
	for (int i = nb; i < nb + 10; ++i) {
		for (int j = 0; j < 3; ++j) {
			uint32_t *addr = esp_snapshot + i*3 + j;
			uint32_t  val  = *addr;
			printf("%p: %X       ", (void*)addr, val);
		}
		printf("\n");
	}
}

void debug(const char *buff) {
    char fmt[5];
    size_t arg1_len = ft_strnlen(buff, ' ');
    const char *priorities[] = {"EMERGENCY", "ALERT", "CRITICAL", "ERROR",
        "WARNING", "NOTICE", "INFO", "DEBUG"};

    for (size_t i = 0; i < 8; i++) {
        size_t prio_len = ft_strlen(priorities[i]);
        if (!ft_strncmp(priorities[i], buff, prio_len) && prio_len == arg1_len) {
            fmt[0] = i + '0';
            ft_strlcpy(fmt + 1, "%s\n", 4);
            printk(fmt, buff + arg1_len + 1);
            break ;
        }
    }
}

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

void info(void) {
    printf("\nCommande      Syntaxe / Usage                         Description\n");
    printf("--------------------------------------------------------------------------\n");
    printf("screen        screen   <1..4>               Selectionne l'ecran 1 a 4\n");
    printf("clear         clear                         Efface l'ecran\n");
    printf("setcolor      setcolor <fcolor> <bcolor>    Change les couleurs texte/fond\n");
    printf("debug         debug    <priority> <msg>     Log de debug\n");
    printf("priority      priority                      Liste des priorites\n");
    printf("stack         stack                         Affiche la stack courante\n");
    printf("reboot        reboot                        Redemarre la machine\n");
    printf("shutdown      shutdown                      Eteint la machine\n");
}

void priority(void)
{
	printf("\nPriority Messages List:\n[EMERGENCY] [ALERT] [CRITICAL] [ERROR] [WARNING] [NOTICE] [INFO] [DEBUG]\n");
}

void handle_cmd() {
    char buff[VGA_WIDTH] = {0};
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
        shutdown();
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
