#include <stdint.h>

#include "led.h"
#include "sleep.h"
#include "systick.h"

int
main(void) {
    init_systick();

    init_led();

    const size_t period = 500;

    while (true) {
        switch_led(true);
        sleep_ms(period);
        switch_led(false);
        sleep_ms(period);
    }
    return 0;
}
