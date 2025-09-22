#pragma once
#include <stddef.h>
#include <stdint.h>
#include "terminal.h"

typedef struct {
	size_t VGA_X[NB_SCREEN];
	size_t VGA_Y[NB_SCREEN];
    const uint8_t end;
    const uint8_t start;
    volatile const size_t addr1;
    volatile const size_t addr2;
} cursor_t;

extern cursor_t cursor;

void initCursor(void);
