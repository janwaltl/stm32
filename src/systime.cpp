#include "systime.hpp"

#include <atomic>

#include "my_assert.hpp"
#include "nucleof411re.hpp"

namespace {
// NOLINTNEXTLINE
std::atomic_bool g_clock_initilized = false;
// NOLINTNEXTLINE
std::atomic_uint32_t g_systick_counter = 0;

static_assert(decltype(g_systick_counter)::is_always_lock_free,
              "we want atomics for the counter");

void
init_systick() {
    // we want 1ms systick, clock is 100MHz -> 1 cycle = 10ns.
    // As per datasheet to get interrupt every N cycles, set it to N-1.
    constexpr std::uint32_t load_value = 100'000 - 1;

    // Stop ticking
    NUCLEO_SYSTICK->ctrl = 0;

    // Reset values
    NUCLEO_SYSTICK->load = load_value - 1;
    NUCLEO_SYSTICK->val = 0;
    g_systick_counter = 0;

    // Start ticking with irqs enabled, ticks at AHB clock rate.
    NUCLEO_SYSTICK->ctrl = 0x7;
}
void
init_pll() {
    // PLL_CLOCK = ((HSI/PLLM)*PLLN)/PLLP
    // HSI is 16MHz, PLL clock input must be <=2MHZ
    // We want 100MHz clock

    NUCLEO_RCC->pllcfgr &= ~(0x3FFFFU); // Zero the reset values
    NUCLEO_RCC->pllcfgr |= 8U << 0;     // PLLM=8
    NUCLEO_RCC->pllcfgr |= 100U << 6;   // PLLN=100
    NUCLEO_RCC->pllcfgr |= 0U << 16;    // PLLP=2 by setting the bits to 0

    // Set prescalers before setting the clock, temporarily makes them slower.
    // Otherwise they could briefly exceed the maximum limit.

    // Set AHB to system clock speed = 100MHz
    NUCLEO_RCC->cfgr &= ~(0xFU << 4);
    // Set APB1 to AHB/2 clock speed = 50Mhz
    NUCLEO_RCC->cfgr &= ~(0x7U << 10);
    NUCLEO_RCC->cfgr |= 0x4 << 10;
    // Set APB2 to AHB clock speed = 100MHz
    NUCLEO_RCC->cfgr &= ~(0x7U << 13);

    // Enable PLL clock
    NUCLEO_RCC->cr |= 1U << 24;

    // Wait until clock is stable and ready
    while ((NUCLEO_RCC->cr & (1U << 25)) == 0)
        ;
}
} // namespace

void
init_sysclock() {
    init_pll();

    //  Enable APB1 clock (where PWR is)
    NUCLEO_RCC->apb1enr |= 1U << 17;
    // Enable AHB1 clock (where FLASH register is)
    NUCLEO_RCC->ahb1enr |= 0x1;

    // Set voltage regulator mode to handle 100MHz
    NUCLEO_PWR->cr |= (0x3U << 14);
    // Set 3 flash wait cycles to handle 100MHz
    NUCLEO_FLASH->acr |= 0x3;

    // Enable instruction, data caches and prefetch
    NUCLEO_FLASH->acr |= 0x7U << 8;

    // Set PLL as system clock
    NUCLEO_RCC->cfgr |= 0x2;
    // Wait until PLL clock is selected
    while (((NUCLEO_RCC->cfgr >> 2) & 0x3U) != 0x2)
        ;

    init_systick();
}

std::chrono::nanoseconds
sysclock_period() {
    using namespace std::chrono_literals;
    my_assert(!g_clock_initilized, "Systime must be initialized");
    // 100MHz clock
    return 10ns;
}

std::chrono::milliseconds
get_systime() {
    return std::chrono::milliseconds{g_systick_counter};
}

extern "C" void
systick_irq_handler(void) {
    g_systick_counter.fetch_add(1);
}
