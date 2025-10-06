#pragma once
#include "idt.h"
#include <stdbool.h>
#include <stdint.h>

void init_keyboard(void);
void keyboard_handler(int_regs_t *regs);

#define SC_LSHIFT   0x2A
#define SC_RSHIFT   0x36
#define SC_ALT      0x38
#define NEW_LINE    0x1C

typedef struct {
    const char *SC_US;
	const char *SC_SHIFT;
	bool shift_on;
	bool alt_on;
	uint8_t scancode;
	bool pressed;
} keyboard_t;


extern const char SCANCODE_INIT_QWERTY[128];
extern const char SCANCODE_SHIFT_INIT_QWERTY[128];
extern const char SCANCODE_INIT_AZERTY[128];
extern const char SCANCODE_SHIFT_INIT_AZERTY[128];

extern keyboard_t keyboard;

void change_kb_layout(const char *layout, const char *layout_shift);
