#pragma once
#include <stddef.h>
#include <stdint.h>

#define  SIGNAL_QUEUE_SIZE  80

typedef struct {
    size_t  int_nb;
    void    *data;
} signal_t;

typedef struct {
    size_t int_nb;
    void *data;
    uint64_t deliver_tick;
} scheduled_signal_t;

typedef struct {
    signal_t    queue[SIGNAL_QUEUE_SIZE];
    size_t      size;
}  signal_stack_t;

typedef struct {
    scheduled_signal_t  queue[SIGNAL_QUEUE_SIZE];
    size_t              size;
} scheduled_signal_list_t;

extern  signal_stack_t  signals;

void init_signal(void);
void schedule_signal(size_t int_nb, void *data, uint64_t delay_ticks);
