#ifndef SYSTICK_HEADER
#define SYSTICK_HEADER

#include <cstddef>

// 1ms tick counter
// NOLINTNEXTLINE
extern volatile size_t g_tick_counter;

void
init_systick();

#endif /* ifndef SYSTICK_HEADER */
