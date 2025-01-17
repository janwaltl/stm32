#include <cstddef>
#include <cstdint>

#include "nucleof411re.hpp"
#include "systick.hpp"

volatile size_t g_tick_counter = 0; // NOLINT

namespace {
// TICK = 1ms
constexpr size_t c_ticks_per_sec = 16000; // 16MHz clock after reset
} // namespace

void
init_systick() {
    // As per datasheet to get interrupt every N cycles, set it to N-1.
    uint32_t load_value = c_ticks_per_sec - 1;

    // Stop ticking
    NUCLEO_SYSTICK->ctrl = 0;

    // Reset values
    NUCLEO_SYSTICK->load = load_value - 1;
    NUCLEO_SYSTICK->val = 0;
    g_tick_counter = 0;

    // Start ticking with irqs enabled, ticks at AHB clock rate.
    NUCLEO_SYSTICK->ctrl = 0x7;
}

extern "C" void
systick_irq_handler(void) {
    g_tick_counter = g_tick_counter + 1;
}
