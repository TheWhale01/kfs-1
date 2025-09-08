#include "julo.h"

void line_break(cursor_t *cursor) {
    cursor->x = 0;
    cursor->y++;
}

void scroll(cursor_t *cursor) {
    (void)cursor;
    return ;
}
