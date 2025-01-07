#include "sleep.hpp"

#include "systick.hpp"

void
sleep_ms(size_t count) {
    size_t ticks = g_tick_counter + count;

    while (g_tick_counter < ticks)
        ;
}
