#include "nvic.hpp"

#include <type_traits>

#include "my_assert.hpp"
#include "nucleof411re.hpp"

void
enable_irq(IRQn line) {
    auto n = static_cast<std::underlying_type_t<IRQn>>(line);
    auto idx = n / 32;
    auto bit = n % 32;

    my_assert(idx < NUCLEO_NVIC->iser.size(), "Out of bounds IRQ");
    NUCLEO_NVIC->iser[idx] = 1U << bit;
}

void
disable_irq(IRQn line) {
    auto n = static_cast<std::underlying_type_t<IRQn>>(line);
    auto idx = n / 32;
    auto bit = n % 32;

    my_assert(idx < NUCLEO_NVIC->iser.size(), "Out of bounds IRQ");
    NUCLEO_NVIC->icer[idx] = 1U << bit;
}
