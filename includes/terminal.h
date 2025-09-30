#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define VGA_HEIGHT 25
#define VGA_WIDTH 80
#define NB_SCREEN 4

typedef enum {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
} vga_color_e;

typedef struct {
	size_t screen;
    uint16_t fcolor;
    uint16_t bcolor;
    volatile uint16_t* const VGA_MEMORY;
	char CMD_BUFFER[NB_SCREEN][VGA_WIDTH];
	uint16_t VGA_SCREEN[NB_SCREEN][VGA_WIDTH * VGA_HEIGHT];
} terminal_t;

extern terminal_t terminal;

void clearscr(void);
void init_terminal(void);
bool check_echappement(char c);
void vga_entry(unsigned char c);

