#include "julo.h"

volatile uint16_t* const VGA_MEMORY = (uint16_t*)0xB8000;

void vga_putchar(int x, int y, char c, uint16_t color) {
    *(VGA_MEMORY + (y * VGA_WIDTH + x)) = (color << 8) | c;
}

void vga_pustring(int x, int y, const char *str, uint16_t color) {
    while (*str) {
        if (x >= VGA_WIDTH || *str == '\n') {
            x = 0;
            y++;
        }
        vga_putchar(x, y, *str++, color);
        x++;
    }
}

int kernel_main(void) {
    const char *str = "Hello, World!!";

    vga_pustring(0, 0, str, VGA_COLOR_WHITE);
    return (0);
}
