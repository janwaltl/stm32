#include <stddef.h>
#include <stdint.h>

#include "led.h"
#include "nucleof411re.h"

void
init_led(void) {
    // Enable GPIO_A peripheral clock.
    NUCLEO_RCC->ahb1enr |= 0x1;
    // Set GPIO A5 into output mode.
    NUCLEO_GPIOA->moder |= (0x1 << (2 * 5));
}

void
switch_led(bool on) {
    // Set GPIO_A5 enable/disable bits
    NUCLEO_GPIOA->bssr = on ? (1U << 5) : (1U << (5 + 16));
}
