#ifndef SYSTICK_HEADER
#define SYSTICK_HEADER

#include <stddef.h>

// 1ms tick counter
// NOLINTNEXTLINE
extern volatile size_t g_tick_counter;

void
init_systick(void);

void
systick_irq_handler(void);

#endif /* ifndef SYSTICK_HEADER */
