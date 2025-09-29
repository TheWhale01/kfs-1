#pragma once
#include "idt.h"

void init_keyboard(void);
void keyboard_handler(int_regs_t *regs);
