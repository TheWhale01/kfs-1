#include "julo.h"
#include "idt.h"
#include "terminal.h"
#include "signal.h"
#include "timer.h"
#include <stdint.h>

extern void load_idt(uint32_t);
extern void enable_interrupts(void);

idt_entry_t idt_entries[IDT_ENTRY_SIZE];
idt_ptr_t idt_ptr;
void *irq_routines[IRQ_ROUTINE_SIZE];
uint8_t panic_stack[STACK_SNAPSHOT_SIZE];

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

static void print_regs(int_regs_t *regs) {
    printf("eax: %x\tecx: %x\tedx: %x\tebx: %x\nesp: %x\tebp: %x\tesi: %x\tedi: %x\n",
        regs->eax, regs->ecx, regs->edx, regs->ebx, regs->esp, regs->ebp, regs->esi, regs->edi);
    printf("cr2: %x\tds:  %x\teip: %x\tcs: %x\neflags: %x\tuseresp: %x\tss: %x\n",
        regs->cr2, regs->ds, regs->eip, regs->cs, regs->eflags, regs->useresp, regs->ss);
}

static void save_stack(int_regs_t *regs) {
    ft_bzero(panic_stack, sizeof(uint8_t) * STACK_SNAPSHOT_SIZE);
    for (size_t i = 0; i < STACK_SNAPSHOT_SIZE; ++i) {
        panic_stack[i] = ((uint8_t *)regs->esp)[i];
        printf("%p: %x\n", (regs->esp + i), panic_stack[i]);
    }
}

static void clean_regs(void) {
    asm volatile (
        "xorl %%eax, %%eax\n\t"
        "xorl %%ebx, %%ebx\n\t"
        "xorl %%ecx, %%ecx\n\t"
        "xorl %%edx, %%edx\n\t"
        "xorl %%esi, %%esi\n\t"
        "xorl %%edi, %%edi\n\t"
        ::: "eax","ebx","ecx","edx", "esi", "edi"
    );
}

void handle_exceptions(int_regs_t *regs) {
    const char *exception_msg[] = { "Division By Zero", "Debug"," Non Maskable Interrupt",
        "Breakpoint", "Into Detected Overflow", "Out of Bounds", "Invalid Opcode",
        "No Coprocessor", "Double fault", "Coprocessor Segment Overrun", "Bad TSS",
        "Segment not present", "Stack fault", "General protection fault", "Page fault",
        "Unknown Interrupt", "Coprocessor Fault", "Alignment Fault", "Machine Check",
        "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved",
        "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved"
    };
    disable_cursor();
    clearscr();
    disable_interrupts();
    printf("System Halted!\nException: %d\n", regs->int_nb);
    printf("%s\n", exception_msg[regs->int_nb]);
    print_regs(regs);
    clean_regs();
    save_stack(regs);
    halt();
}

void handle_irq(int_regs_t *regs) {
    void (*handler)(int_regs_t *regs);
    handler = irq_routines[signals.queue[signals.size].int_nb - 32];
    if (!handler) {
        printf("Unknown handler for interrupt request: %d", signals.queue[signals.size].int_nb);
        return ;
    }
    handler(regs);
    if (regs->int_nb >= 40)
        outb(PIC2_COMMAND_PORT, PIC_CMD_EOI);
    outb(PIC1_COMMAND_PORT, PIC_CMD_EOI);
}

void isr_handler(int_regs_t *regs) {
    if (regs->int_nb < 32) {
        handle_exceptions(regs);
        return ;
    }
    add_signal(regs->int_nb, regs);
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
