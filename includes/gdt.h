#pragma once
#include <stddef.h>
#include <stdint.h>

#define GDT_ENTRY_SIZE 7

typedef struct gdt_entry {
	uint16_t	limit;
	uint16_t	base_low;
	uint8_t		base_middle;
	uint8_t		access;
	uint8_t		flags;
	uint8_t		base_high;
} __attribute__((packed)) gdt_entry_t;

typedef struct gdt_ptr {
	uint16_t		limit;
	unsigned int	base;
} __attribute__((packed)) gdt_ptr_t;

extern gdt_entry_t gdt_entries[GDT_ENTRY_SIZE] __attribute__((section(".gdt")));
extern gdt_ptr_t   gdt_ptr;

void init_gdt(void);
void setgdt_entry(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags);
