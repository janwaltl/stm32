#include <array>
#include <cstdint>

#include "led.hpp"
#include "serial.hpp"
#include "sleep.hpp"
#include "systick.hpp"

int
notmain() {
    init_systick();

    init_led();

    init_serial();

    std::array<char, 256> buffer;
    while (true) {

        size_t n = serial_receive(buffer.data(), buffer.size() - 1);
        buffer[n] = '\0';

        serial_send_str("Received ");
        char ddd = '0' + static_cast<char>((n / 100) % 10);
        char dd = '0' + static_cast<char>((n / 10) % 10);
        char d = '0' + static_cast<char>(n % 10);
        serial_send_char(ddd);
        serial_send_char(dd);
        serial_send_char(d);
        serial_send_str(" bytes:\n>\"");
        serial_send_str(buffer.data());
        serial_send_str("\"\n");
    }

    return 0;
}
