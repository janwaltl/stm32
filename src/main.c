#include <stdint.h>

#include "led.h"
#include "serial.h"
#include "sleep.h"
#include "systick.h"

int
main(void) {
    init_systick();

    init_led();

    init_serial();

    char buffer[256];
    while (true) {

        size_t n = serial_receive(buffer, sizeof(buffer) - 1);
        buffer[n] = '\0';

        serial_send_str("Received ");
        char ddd = '0' + (char)((n / 100) % 10);
        char dd = '0' + (char)((n / 10) % 10);
        char d = '0' + (char)(n % 10);
        serial_send_char(ddd);
        serial_send_char(dd);
        serial_send_char(d);
        serial_send_str(" bytes:\n>\"");
        serial_send_str(buffer);
        serial_send_str("\"\n");
    }

    return 0;
}
