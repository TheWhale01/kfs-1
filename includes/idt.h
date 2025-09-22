#pragma once
#include <stdint.h>
#include <stddef.h>

#define IDT_ENTRY_SIZE 256

// https://wiki.osdev.org/Interrupt_Descriptor_Table
// __attribute__((packed)) means that the compiler will compile the structure as
// is in assembly without any sort of optimization. This will ensure the good
// functioning of the structure
typedef struct {
    uint16_t offset_1;
    uint16_t selector;
    uint8_t zero;
    uint8_t type_attributes;
    uint16_t offset_2;
} __attribute((packed)) idt_entry_t;

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_ptr_t;

extern idt_entry_t idt_entries[IDT_ENTRY_SIZE];
extern idt_ptr_t   idt_ptr;

void init_idt(void);
void set_idt_gate(uint8_t num, uint32_t base, uint16_t selector, uint8_t flags);
