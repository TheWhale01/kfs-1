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

typedef struct {
    uint32_t cr2;
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx,  eax;
    uint32_t int_nb, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
} __attribute__((packed)) int_regs_t;

typedef enum {
    IDT_FLAG_GATE_TASK          = 0x5,
    IDT_FLAG_GATE_16BIT_INT     = 0x6,
    IDT_FLAG_GATE_16BIT_TRAP    = 0x7,
    IDT_FLAG_GATE_32BIT_INT     = 0xE,
    IDT_FLAG_GATE_32BIT_TRAP    = 0xF,
    IDT_FLAG_RING0              = (0 << 5),
    IDT_FLAG_RING1              = (1 << 5),
    IDT_FLAG_RING2              = (2 << 5),
    IDT_FLAG_RING3              = (3 << 5),
    IDT_FLAG_PRESENT            = 0x80,
} idt_flags_e;

extern idt_entry_t idt_entries[IDT_ENTRY_SIZE];
extern idt_ptr_t   idt_ptr;

void init_idt(void);
void init_idt_gates(void);
void enable_idt_gate(int int_nb);
void disable_idt_gate(int int_nb);
void set_idt_gate(uint8_t num, void *base, uint16_t selector, uint8_t flags);

void isr_handler(int_regs_t *regs);
