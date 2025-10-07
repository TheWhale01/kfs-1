#pragma once

#include "idt.h"
#include <stdint.h>

#define MAX_SYSCALL_NB	80

typedef int (*syscall_t)(int_regs_t *regs);

void init_syscalls(void);

int handle_syscall(int_regs_t *regs);
