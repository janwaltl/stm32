#include "serial.h"

#include <stdbool.h>

#include "led.h"
#include "nucleof411re.h"

void
init_serial(void) {
    // Enable USART2 peripheral clock.
    NUCLEO_RCC->apb1enr |= 1U << 17;
    // Enable GPIO_A peripheral clock.
    NUCLEO_RCC->ahb1enr |= 0x1;

    // Set GPIO_A 2,3 alternative function
    NUCLEO_GPIOA->moder |= 0x2 << (2 * 2);
    NUCLEO_GPIOA->moder |= 0x2 << (2 * 3);
    // Select UART as alternative function
    NUCLEO_GPIOA->afrl |= 0x7 << (4 * 2);
    NUCLEO_GPIOA->afrl |= 0x7 << (4 * 3);

    // Setup USART2 baudrate
    // 104.166666
    // 104=0x67
    // .16 << 4 = 2.56 ~ 3 = 0x3
    NUCLEO_USART2->brr = 0x683;
    // Enable UART, disable all interrupts, parity...
    NUCLEO_USART2->cr1 = (0x1 << 13);

    // Set 1 stop bits, disable the rest
    NUCLEO_USART2->cr2 = 0;
    // Disable extensions
    NUCLEO_USART2->cr3 = 0;
}

static bool
rx_empty(uint32_t status_reg) {
    return (status_reg & (1 << 5)) == 0;
}
static bool
rx_idle(uint32_t status_reg) {
    return (status_reg & (1 << 4)) != 0;
}

static bool
tx_empty(uint32_t status_reg) {
    return (status_reg & (1 << 7)) != 0;
}
static bool
tx_complete(uint32_t status_reg) {
    return (status_reg & (1 << 6)) != 0;
}

void
serial_send_str(const char *msg) {
    // Enable TX
    NUCLEO_USART2->cr1 |= (1 << 3);
    switch_led(true);
    while (*msg) {
        while (!tx_empty(NUCLEO_USART2->sr))
            ;

        // Send Data
        NUCLEO_USART2->dr = *msg++;
    }
    while (!tx_complete(NUCLEO_USART2->sr))
        ;
    switch_led(false);
    // Disable TX
    NUCLEO_USART2->cr1 &= ~(1U << 3);
}

void
serial_send_char(char c) {
    // Enable TX
    NUCLEO_USART2->cr1 |= (1 << 3);
    switch_led(true);
    while (!tx_empty(NUCLEO_USART2->sr))
        ;
    // Send Data
    NUCLEO_USART2->dr = c;
    while (!tx_complete(NUCLEO_USART2->sr))
        ;
    switch_led(false);
    // Disable TX
    NUCLEO_USART2->cr1 &= ~(1U << 3);
}

size_t
serial_receive(char *dst, size_t n) {
    size_t i = 0;

    // Enable RX
    NUCLEO_USART2->cr1 |= (1 << 2);
    switch_led(true);

    uint32_t status = NUCLEO_USART2->sr;
    while (i < n && (i == 0 || !rx_empty(status) || !rx_idle(status))) {

        if (!rx_empty(status)) {
            dst[i++] = (uint8_t)(NUCLEO_USART2->dr & 0xFF);
        }
        status = NUCLEO_USART2->sr;
    }

    switch_led(false);
    // Disable RX
    NUCLEO_USART2->cr1 &= ~(1U << 2);
    return i;
}
