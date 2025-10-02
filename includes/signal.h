#pragma once
#include <stddef.h>

#define  SIGNAL_QUEUE_SIZE  80

typedef struct {
    size_t  int_nb;
    void    *data;
} signal_t;

typedef struct {
    signal_t    queue[SIGNAL_QUEUE_SIZE];
    size_t      head;
    size_t      tail;
}  signal_stack_t;

extern  signal_stack_t  signals;

void init_signal(void);
