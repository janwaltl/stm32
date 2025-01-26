#include "led.hpp"
#include "my_assert.hpp"
#include "nvic.hpp"
#include "serial.hpp"
#include "sleep.hpp"
#include "systime.hpp"

namespace {
// NOLINTNEXTLINE
SerialDev *volatile g_serial;

void
init_usart2() {
    // TODO(me): move to clock API
    //  Enable USART2 peripheral clock.
    NUCLEO_RCC->apb1enr |= 1U << 17;
    // Enable GPIO_A peripheral clock.
    NUCLEO_RCC->ahb1enr |= 0x1;

    // TODO(me): move to gpio API
    //  Set GPIO_A 2,3 alternative function
    NUCLEO_GPIOA->moder |= 0x2 << (2 * 2);
    NUCLEO_GPIOA->moder |= 0x2 << (2 * 3);
    // Select UART as alternative function
    NUCLEO_GPIOA->afrl |= 0x7 << (4 * 2);
    NUCLEO_GPIOA->afrl |= 0x7 << (4 * 3);
    // Set to hispeed
    NUCLEO_GPIOA->ospeedr |= 0x3 << (2 * 2);
    NUCLEO_GPIOA->ospeedr |= 0x3 << (2 * 3);

    *(volatile uint32_t *)(0x40013800 + 0x20) |= 1;
}

void
echo(SerialDev &serial) {
    std::array<char, 256> buffer;
    while (true) {

        size_t n = serial.recv(buffer.data(), buffer.size() - 1);
        buffer[n] = '\0';

        serial.send("Received ");
        char ddd = '0' + static_cast<char>((n / 100) % 10);
        char dd = '0' + static_cast<char>((n / 10) % 10);
        char d = '0' + static_cast<char>(n % 10);
        serial.send(ddd);
        serial.send(dd);
        serial.send(d);
        serial.send(" bytes:\n>\"");
        serial.send(buffer.data());
        serial.send("\"\n");
    }
}
} // namespace

extern "C" void
usart2_irq_handler(void) {
    switch_led(true);
    if (g_serial)
        g_serial->irq_handler();
    switch_led(false);
}

int
notmain() {
    init_sysclock();
    init_led();
    init_usart2();

    while (false) {
        using namespace std::chrono_literals;
        sleep_ms(500ms);
        switch_led(true);
        sleep_ms(500ms);
        switch_led(false);
    }

    SerialDev serial{NUCLEO_USART2};
    g_serial = &serial;

    enable_irq(IRQn::USART2);

    serial.send("Hello ");
    serial.send("World\n");

    echo(*g_serial);

    return 0;
}

// TODO
//  1. Switch to 100MHz clock, proper clock system, including systick
//  - set PLLM to 8 -> 2MHz clock input
//  - set PLLN to 100 -> 200MHz VCO
//  - set PLLP to 2 -> 100MHz clock output
//  - switch system clock source to PLL
//  1. Switch to 115200 baud rate
//  1. Proper boot phase of the board, which initializes serial console.
//  1. printk, logging behaviour.
