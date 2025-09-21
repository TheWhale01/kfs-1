#include "julo.h"
#include <stdint.h>

extern void idt_flush(uint32_t);

void initGDT(idt_t *idt) {
    idt->ptr->limit = sizeof(idt_entry_t) * IDT_SIZE - 1;
    idt->ptr->base = (uint32_t)&idt->entries;
    ft_bzero(idt->entries, sizeof(idt_entry_t) * IDT_SIZE);
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

    setIDTGate(0, (uint32_t)irs0, 0x08, 0x8E);
}

void setIDTGate(uint8_t num, uint32_t base, uint16_t selector, uint8_t flags) {
}
