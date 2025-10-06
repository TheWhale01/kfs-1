#include "idt.h"
#include "julo.h"
#include <stdint.h>

inline uint8_t inb(uint16_t port) {
    uint8_t ret;

    __asm__ volatile (
        "inb %w1, %b0"
        : "=a"(ret)
        : "Nd"(port)
        : "memory"
    );
    return ret;
}

inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

inline void outw(uint16_t port, uint16_t val) {
    __asm__ volatile ("outw %0, %1" : : "a"(val), "d"(port));
}

void reboot(void) {
    idt_ptr_t ptr = {0, 0};

    disable_cursor();
    disable_interrupts();
    asm volatile ("lidt (%0)" : : "r"(&ptr));
    asm volatile ("int $3");
}

void shutdown(void) {
    // Only works on QEMU
    disable_cursor();
    outw(0x604, 0x2000);
}

void disable_interrupts(void) {
    asm volatile ("cli");
}

void halt(void) {
    asm volatile ("hlt");
}
