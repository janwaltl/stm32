#include "sleep.hpp"

#include "systime.hpp"

void
sleep_ms(std::chrono::milliseconds count) {
    auto ticks = get_systime() + count;

    while (get_systime() < ticks)
        ;
}
