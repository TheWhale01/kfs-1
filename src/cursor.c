#include "julo.h"

void line_break(cursor_t *cursor) {
    cursor->x = 0;
    cursor->y = 0;
}

void scroll(cursor_t *cursor) {
    (void)cursor;
    return ;
}
