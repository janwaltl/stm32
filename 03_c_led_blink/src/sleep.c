#include "sleep.h"
#include "systick.h"

void
sleep_ms(size_t count) {
    size_t ticks = g_tick_counter + count;

    while (g_tick_counter < ticks)
        ;
}
