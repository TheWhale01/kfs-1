#include "julo.h"
#include "idt.h"
#include "terminal.h"
#include <stdint.h>

extern void load_idt(uint32_t);

idt_entry_t idt_entries[IDT_ENTRY_SIZE];
idt_ptr_t idt_ptr;

void init_idt() {
    idt_ptr.limit = sizeof(idt_entry_t) * IDT_ENTRY_SIZE - 1;
    idt_ptr.base = (uint32_t)&idt_entries;
    ft_bzero(idt_entries, sizeof(idt_entry_t) * IDT_ENTRY_SIZE);

    load_idt((uint32_t)&idt_ptr);
    init_idt_gates();
    for (size_t i = 0; i < IDT_ENTRY_SIZE; i++)
        enable_idt_gate(i);

    // Initialization du PIC
    // 0x20 Maitre commande
    // 0xA0 Maitre data
    // 0x21 Esclave commande
    // 0xA1 Esclave data
    // outb(0x20, 0x11);
    // outb(0xA0, 0x11);

    // outb(0x21, 0x20);
    // outb(0xA1, 0x28);

    // outb(0x21, 0x04);
    // outb(0xA1, 0x02);

    // outb(0x21, 0x01);
    // outb(0xA1, 0x01);

    // outb(0x21, 0x0);
    // outb(0xA1, 0x0);

    // init_idt_gates();
}

void set_idt_gate(uint8_t num, void *base, uint16_t selector, uint8_t flags) {
    idt_entries[num].offset_1 = ((uint32_t)base) & 0xffff;
    idt_entries[num].selector = selector;
    idt_entries[num].zero = 0;
    idt_entries[num].type_attributes = flags | 0x60;
    idt_entries[num].offset_2 = (((uint32_t)base) >> 16) & 0xffff;
}

void isr_handler(int_regs_t *regs) {
    (void)regs;
    clearscr();
    printf("Une interruption est arrivee !\n");
    while (true);
}

void enable_idt_gate(int int_nb) {
    SET_FLAG(idt_entries[int_nb].type_attributes, IDT_FLAG_PRESENT);
}

void disable_idt_gate(int int_nb) {
    UNSET_FLAG(idt_entries[int_nb].type_attributes, IDT_FLAG_PRESENT);
}
