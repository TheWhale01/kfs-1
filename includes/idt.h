#pragma once
#include <stdint.h>
#include <stddef.h>

#define IDT_ENTRY_SIZE 256
#define IRQ_ROUTINE_SIZE 16

#define PIC1_COMMAND_PORT   0x20
#define PIC1_DATA_PORT      0x21
#define PIC2_COMMAND_PORT   0xA0
#define PIC2_DATA_PORT      0xA1

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

typedef enum {
    PIC_ICW1_ICW4           = 0x01,
    PIC_ICW1_INITIALIZE     = 0x10,
} pic_icw1_e;

typedef enum {
    PIC_ICW4_8096                   = 0x1,
    PIC_ICW4_AUTO_EOI               = 0x2,
    PIC_ICW4_BUFFER_MASTER          = 0x4,
    PIC_ICW4_BUFFER_SLAVE           = 0x0,
    PIC_ICW4_BUFFERED               = 0x8,
    PIC_ICW4_SFNM                   = 0x10
} pic_icw4_e;

typedef enum {
    PIC_CMD_EOI = 0x20,
} pic_cmd_e;

extern idt_entry_t idt_entries[IDT_ENTRY_SIZE];
extern idt_ptr_t   idt_ptr;
extern void        *irq_routines[IRQ_ROUTINE_SIZE];


void init_idt(void);
void init_idt_gates(void);
void enable_idt_gate(int int_nb);
void disable_idt_gate(int int_nb);
void irq_uninstall_handler(int irq);
void irq_install_handler(int irq, void (*handler)(int_regs_t *regs));
void set_idt_gate(uint8_t num, void *base, uint16_t selector, uint8_t flags);

void isr_handler(int_regs_t *regs);
