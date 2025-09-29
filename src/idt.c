#include "julo.h"
#include "idt.h"
#include "terminal.h"
#include <stdint.h>

extern void load_idt(uint32_t);
extern void enable_interrupts(void);

idt_entry_t idt_entries[IDT_ENTRY_SIZE];
idt_ptr_t idt_ptr;
void *irq_routines[IRQ_ROUTINE_SIZE];

static void configure_pic(uint8_t offset1, uint8_t offset2) {
    // Init control word 1
    outb(PIC1_COMMAND_PORT, PIC_ICW1_ICW4 | PIC_ICW1_INITIALIZE);
    outb(PIC2_COMMAND_PORT, PIC_ICW1_ICW4 | PIC_ICW1_INITIALIZE);

    // Init control word 2
    outb(PIC1_DATA_PORT, offset1);
    outb(PIC2_DATA_PORT, offset2);

    // Init control word 3
    outb(PIC1_DATA_PORT, 0x4);
    outb(PIC2_DATA_PORT, 0x2);

    // Init control word 4
    outb(PIC1_DATA_PORT, PIC_ICW4_8096);
    outb(PIC2_DATA_PORT, PIC_ICW4_8096);

    // clear data ports
    outb(PIC1_DATA_PORT, 0);
    outb(PIC2_DATA_PORT, 0);

    // Mask IRQ0 (No PIT)
    uint8_t mask = inb(PIC1_DATA_PORT);
    mask |= 0x01;
    outb(PIC1_DATA_PORT, mask);
}

void init_idt() {
    idt_ptr.limit = sizeof(idt_entry_t) * IDT_ENTRY_SIZE - 1;
    idt_ptr.base = (uint32_t)&idt_entries;
    ft_bzero(idt_entries, sizeof(idt_entry_t) * IDT_ENTRY_SIZE);
    ft_bzero(irq_routines, IRQ_ROUTINE_SIZE);

    load_idt((uint32_t)&idt_ptr);
    init_idt_gates();
    for (size_t i = 0; i < IDT_ENTRY_SIZE; i++)
        enable_idt_gate(i);
    configure_pic(0x20, 0x28);
    enable_interrupts();
}

void set_idt_gate(uint8_t num, void *base, uint16_t selector, uint8_t flags) {
    idt_entries[num].offset_1 = ((uint32_t)base) & 0xffff;
    idt_entries[num].selector = selector;
    idt_entries[num].zero = 0;
    idt_entries[num].type_attributes = flags | 0x60;
    idt_entries[num].offset_2 = (((uint32_t)base) >> 16) & 0xffff;
}

void isr_handler(int_regs_t *regs) {
    const char *exception_msg[] = { "Division By Zero", "Debug"," Non Maskable Interrupt",
        "Breakpoint", "Into Detected Overflow", "Out of Bounds", "Invalid Opcode",
        "No Coprocessor", "Double fault", "Coprocessor Segment Overrun", "Bad TSS",
        "Segment not present", "Stack fault", "General protection fault", "Page fault",
        "Unknown Interrupt", "Coprocessor Fault", "Alignment Fault", "Machine Check",
        "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved",
        "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved"
    };
    if (regs->int_nb < 32) {
        // Interruption is an excpetion
        clearscr();
        printf("System Halted!\nException: %d\n", regs->int_nb);
        printf("%s", exception_msg[regs->int_nb]);
        while(true);
    }
    else if (regs->int_nb >= 32 && regs->int_nb <= 47) {
        // Interruption is an IRQ
        void (*handler)(int_regs_t *regs);

        handler = irq_routines[regs->int_nb - 32];
        if (!handler) {
            printf("Unknown handler for interrupt request: %d", regs->int_nb);
            return ;
        }
        handler(regs);
        if (regs->int_nb >= 40)
            outb(PIC2_COMMAND_PORT, PIC_CMD_EOI);
        outb(PIC1_COMMAND_PORT, PIC_CMD_EOI);
    }
}

void enable_idt_gate(int int_nb) {
    SET_FLAG(idt_entries[int_nb].type_attributes, IDT_FLAG_PRESENT);
}

void disable_idt_gate(int int_nb) {
    UNSET_FLAG(idt_entries[int_nb].type_attributes, IDT_FLAG_PRESENT);
}

void irq_install_handler(int irq, void (*handler)(int_regs_t *regs)) {
    irq_routines[irq] = handler;
}

void irq_uninstall_handler(int irq) {
    irq_routines[irq] = 0;
}
