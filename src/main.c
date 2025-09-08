#include "julo.h"

int kernel_main(void) {
    cursor_t cursor = { .x = 0, .y = 0 };
    terminal_t terminal = {
        .cursor = &cursor,
        .fcolor = VGA_COLOR_WHITE,
        .bcolor = VGA_COLOR_BLACK,
        .VGA_MEMORY = (volatile uint16_t*)0xB8000,
    };

    printk(&terminal, KERN_EMERG "%p\n", &cursor);
    printk(&terminal, KERN_ALERT "%d\n", 13);
    printk(&terminal, KERN_CRIT "%i\n", 12356543);
    printk(&terminal, KERN_ERR "%u\n", 4294967295);
    printk(&terminal, KERN_WARNING "%c\n", 'G');
    printk(&terminal, KERN_NOTICE "%s\n", "BONSOIR JE NE SUIS PAS MOI");
    printk(&terminal, KERN_INFO "%x\n", 15);
    printk(&terminal, KERN_DEBUG "%X\n", 15);
    printk(&terminal, "IMPOSTEUR: %%\n");
    return (0);
}
