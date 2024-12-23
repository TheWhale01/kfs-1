#include "kfs.h"

inline uint8_t inb(uint16_t port)
{
	uint8_t _v;

	__asm__ volatile ("inb %w1,%0":"=a" (_v):"Nd" (port));
	return _v;
}

inline void outb(uint16_t port, uint8_t value)
{
	__asm__ volatile ("outb %b0,%w1": :"a" (value), "Nd" (port));
}

size_t strlen(const char *str) {
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}
