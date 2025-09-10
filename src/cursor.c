#include "julo.h"
#include <stdint.h>

void line_break(cursor_t *cursor) {
    cursor->x = 0;
    cursor->y++;
    update_cursor(cursor);
}

void scroll(cursor_t *cursor) {
    (void)cursor;
    return ;
}

void enable_cursor(const cursor_t *cursor) {
    outb(cursor->addr1, 0x0A);
    outb(cursor->addr2, (inb(cursor->addr2) & 0xC0) | cursor->start);
    outb(cursor->addr1, 0x0B);
    outb(cursor->addr2, (inb(cursor->addr2) & 0xE0) | cursor->end);
}

void disable_cursor(const cursor_t *cursor) {
    outb(cursor->addr1, 0x0A);
    outb(cursor->addr2, 0x20);
}

void update_cursor(const cursor_t *cursor) {
    uint16_t pos = cursor->y * VGA_WIDTH + cursor->x;

    outb(cursor->addr1, 0x0F);
    outb(cursor->addr2, (uint8_t)(pos & 0xFF));
    outb(cursor->addr1, 0x0E);
    outb(cursor->addr2, (uint8_t)((pos >> 8) & 0xFF));
}
