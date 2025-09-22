#include "gdt.h"
#include "julo.h"

extern void gdt_flush(uint32_t);
gdt_entry_t gdt_entries[GDT_ENTRY_SIZE] __attribute__((section(".gdt")));
gdt_ptr_t   gdt_ptr;

void init_gdt(void) {

	gdt_ptr.limit = (sizeof(gdt_entry_t) * 5) - 1;
	gdt_ptr.base = (uint32_t)&gdt_entries;

	setgdt_entry(0, 0, 0, 0, 0); // Null segment
	setgdt_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Kernel Code segment 9A = 1001 1010
	setgdt_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Kernel Data segment 92 = 1001 0010
	setgdt_entry(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User Code segment FA = 1111 1010
	setgdt_entry(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User Data segment F2 = 1111 0010 // 0xCF = 1100 1111
	setgdt_entry(5, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User Data
	setgdt_entry(6, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User Stack
	gdt_flush((uint32_t)&gdt_ptr);
}

void setgdt_entry(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags) {
	gdt_entries[num].base_low    = (base & 0xFFFF);
	gdt_entries[num].base_middle = (base >> 16) & 0xFF;
	gdt_entries[num].base_high   = (base >> 24) & 0xFF;

	gdt_entries[num].limit       = (limit & 0xFFFF);
	gdt_entries[num].flags       = (limit >> 16) & 0x0F;

	gdt_entries[num].flags      |= flags & 0xF0;
	gdt_entries[num].access      = access;
}
