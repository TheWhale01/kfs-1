#include "syscalls.h"
#include "idt.h"
#include "julo.h"
#include "libft.h"

syscall_t syscall_routines[MAX_SYSCALL_NB];

int sys_write(int_regs_t *regs) {
    int fd = regs->ebx;
    const char *buf = (const char *)regs->ecx;
    uint32_t size =  regs->edx;

    if (fd != 1)
        return -1;
    for (size_t i = 0; i < size; i++)
        vga_putchar(buf[i]);
    return size;
}

void init_syscalls(void) {
    ft_bzero(syscall_routines, sizeof(syscall_t) * MAX_SYSCALL_NB);

    syscall_routines[0] = sys_write; // -> Need to define sys_write
}

int handle_syscall(int_regs_t *regs) {
    if (regs->eax >= MAX_SYSCALL_NB || !syscall_routines[regs->eax])
        return -1;
    return syscall_routines[regs->eax](regs);
}
