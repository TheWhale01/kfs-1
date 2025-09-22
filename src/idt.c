#include "julo.h"
#include <stdint.h>

extern void idt_flush(uint32_t);

void initGDT(void) {
    idt_ptr.limit = sizeof(idt_entry_t) * IDT_SIZE - 1;
    idt_ptr.base = (uint32_t)&idt_entries;
    ft_bzero(idt_entries, sizeof(idt_entry_t) * IDT_SIZE);
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);

    set_idt_gate(0, (uint32_t)irs0, 0x08, 0x8E);
}

void set_idt_gate(uint8_t num, uint32_t base, uint16_t selector, uint8_t flags) {
}
