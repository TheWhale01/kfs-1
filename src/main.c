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

	// char s2000[VGA_WIDTH * VGA_HEIGHT + 1];

	// // term 0
	// printk("bonsoir je ne suis pas moi\n");
	// printf("42\n");

	// // term 1

	// for (size_t i = 0; i < VGA_WIDTH; i++)
    //     vga_putchar('A');
	// make_pattern_2000(s2000);
	// vga_putstring(s2000);
	// vga_putstring("i\brrrrrrrrrrrrrrrrrrrrrr\trrrrrrrrrrrrrerr\vrrrrrrrrrrss\fsssssssssss\asssssssssssssssssssse\nssse\rsex");

	// // term 2
	// change_screen(2);
	// vga_putstring("ssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss\t");

	// // term 0
	// change_screen(0);
	// for (int i = 0; i < 80; i++)
	// 	vga_putstring("1");
	// vga_putstring("\r");
	// for (int i = 0; i < 80; i++)
	//     vga_putstring("2");

	// for (int i = 0; i < 160; i++)
	// 	vga_putstring("2");
    // printk(KERN_EMERG "%p\n", &cursor);
    // printk(KERN_ALERT "%d\n", 13);
    // printk(KERN_CRIT "%i\n", 12356543);
    // printk(KERN_ERR "%u\n", 4294967295);
    // printk(KERN_WARNING "%c\n", 'G');
    // printk(KERN_NOTICE "%s\n", "BONSOIR JE NE SUIS PAS MOI");
    // printk(KERN_INFO "%x\n", 15);
    // printk(KERN_DEBUG "%X\n", 15);
    // printk("IMPOSTEUR: %%\n");

    // Tester les interruptions
    // Division par 0
    // int a = 10, d = 0, q;
    // asm volatile(
    //     "xorl %%edx, %%edx\n\t"   /* EDX = 0 (haut du dividende) */
    //     "movl %1, %%eax\n\t"      /* placer dividende en EAX */
    //     "divl %2\n\t"             /* divise EDX:EAX par %2 -> diviseur 0 provoque exception */
    //     : "=a"(q)                 /* sortie : quotient dans EAX */
    //     : "r"(a), "r"(d)          /* entrées */
    // );
    return (0);
}
