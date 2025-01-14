#include <cstddef>
#include <cstdint>

// Set by linker script
extern "C" unsigned char _bdata;      // NOLINT
extern "C" unsigned char _edata;      // NOLINT
extern "C" unsigned char _bbss;       // NOLINT
extern "C" unsigned char _ebss;       // NOLINT
extern "C" unsigned char _ldbdata;    // NOLINT
extern "C" unsigned char _binit_data; // NOLINT
extern "C" unsigned char _einit_data; // NOLINT
extern "C" unsigned char _estack;     // NOLINT

#define DEFAULT __attribute__((weak, alias("default_irq_handler")))

extern int
notmain();

extern "C" {
void DEFAULT
systick_irq_handler(void);

void DEFAULT
usart2_irq_handler(void);
}

static void
halt() {
    __asm__ __volatile__("halt_loop:\n\t"
                         "LDR  r0, =0xDEADC0DE\n\t"
                         "LDR  r1, =0xDEADC0DE\n\t"
                         "LDR  r2, =0xDEADC0DE\n\t"
                         "LDR  r3, =0xDEADC0DE\n\t"
                         "B halt_loop");
}

// Call all compiler-generated init functions.
static void
startup_init_data_init() {
    using init_fun = void (*)();

    auto *p = reinterpret_cast<init_fun *>(&_binit_data);
    while (p < reinterpret_cast<init_fun *>(&_einit_data)) {
        (*p++)();
    }
}

// copy .data section from FLASH to RAM
static void
startup_data_init() {
    volatile unsigned char *p = &_bdata;
    volatile unsigned char *end = &_edata;
    volatile unsigned char *from = &_ldbdata;

    while (p < end) {
        *p++ = *from++;
    }
}

// zero-out .bss section in RAM
static void
startup_bss_init() {
    volatile unsigned char *p = &_bbss;

    while (p < &_ebss) {
        *p++ = 0;
    }
}

// Entry point
extern "C" void
reset_irq_handler() {
    startup_data_init();
    startup_bss_init();
    startup_init_data_init();

    notmain();
    halt();
}

extern "C" void
default_irq_handler() {
    halt();
}

using irq_t = void (*)();

extern "C" __attribute__((used, section(".vtable")))
const irq_t reset_table[101] = {
    reinterpret_cast<irq_t>(&_estack), // Initial stack pointer
    &reset_irq_handler,                // Reset handler
    &default_irq_handler,              // NMI
    &default_irq_handler,              // Hard fault
    &default_irq_handler,              // Memory fault
    &default_irq_handler,              // Bus fault
    &default_irq_handler,              // Usage fault
    nullptr,                           // Reserved
    nullptr,                           // Reserved
    nullptr,                           // Reserved
    nullptr,                           // Reserved
    &default_irq_handler,              // SVCall
    nullptr,                           // Reserved
    nullptr,                           // Reserved
    &default_irq_handler,              // PendSV
    &systick_irq_handler,              // Systick
    &default_irq_handler,              // IRQ 00
    &default_irq_handler,              // IRQ 01
    &default_irq_handler,              // IRQ 02
    &default_irq_handler,              // IRQ 03
    &default_irq_handler,              // IRQ 04
    &default_irq_handler,              // IRQ 05
    &default_irq_handler,              // IRQ 06
    &default_irq_handler,              // IRQ 07
    &default_irq_handler,              // IRQ 08
    &default_irq_handler,              // IRQ 09
    &default_irq_handler,              // IRQ 10
    &default_irq_handler,              // IRQ 11
    &default_irq_handler,              // IRQ 12
    &default_irq_handler,              // IRQ 13
    &default_irq_handler,              // IRQ 14
    &default_irq_handler,              // IRQ 15
    &default_irq_handler,              // IRQ 16
    &default_irq_handler,              // IRQ 17
    &default_irq_handler,              // IRQ 18
    &default_irq_handler,              // IRQ 19
    &default_irq_handler,              // IRQ 20
    &default_irq_handler,              // IRQ 21
    &default_irq_handler,              // IRQ 22
    &default_irq_handler,              // IRQ 23
    &default_irq_handler,              // IRQ 24
    &default_irq_handler,              // IRQ 25
    &default_irq_handler,              // IRQ 26
    &default_irq_handler,              // IRQ 27
    &default_irq_handler,              // IRQ 28
    &default_irq_handler,              // IRQ 29
    &default_irq_handler,              // IRQ 30
    &default_irq_handler,              // IRQ 31
    &default_irq_handler,              // IRQ 32
    &default_irq_handler,              // IRQ 33
    &default_irq_handler,              // IRQ 34
    &default_irq_handler,              // IRQ 35
    &default_irq_handler,              // IRQ 36
    &default_irq_handler,              // IRQ 37
    &usart2_irq_handler,               // IRQ 38
    &default_irq_handler,              // IRQ 39
    &default_irq_handler,              // IRQ 40
    &default_irq_handler,              // IRQ 41
    &default_irq_handler,              // IRQ 42
    &default_irq_handler,              // IRQ 43
    &default_irq_handler,              // IRQ 44
    &default_irq_handler,              // IRQ 45
    &default_irq_handler,              // IRQ 46
    &default_irq_handler,              // IRQ 47
    &default_irq_handler,              // IRQ 48
    &default_irq_handler,              // IRQ 49
    &default_irq_handler,              // IRQ 50
    &default_irq_handler,              // IRQ 51
    &default_irq_handler,              // IRQ 52
    &default_irq_handler,              // IRQ 53
    &default_irq_handler,              // IRQ 54
    &default_irq_handler,              // IRQ 55
    &default_irq_handler,              // IRQ 56
    &default_irq_handler,              // IRQ 57
    &default_irq_handler,              // IRQ 58
    &default_irq_handler,              // IRQ 59
    &default_irq_handler,              // IRQ 60
    &default_irq_handler,              // IRQ 61
    &default_irq_handler,              // IRQ 62
    &default_irq_handler,              // IRQ 63
    &default_irq_handler,              // IRQ 64
    &default_irq_handler,              // IRQ 65
    &default_irq_handler,              // IRQ 66
    &default_irq_handler,              // IRQ 67
    &default_irq_handler,              // IRQ 68
    &default_irq_handler,              // IRQ 69
    &default_irq_handler,              // IRQ 70
    &default_irq_handler,              // IRQ 71
    &default_irq_handler,              // IRQ 72
    &default_irq_handler,              // IRQ 73
    &default_irq_handler,              // IRQ 74
    &default_irq_handler,              // IRQ 75
    &default_irq_handler,              // IRQ 76
    &default_irq_handler,              // IRQ 77
    &default_irq_handler,              // IRQ 78
    &default_irq_handler,              // IRQ 79
    &default_irq_handler,              // IRQ 80
    &default_irq_handler,              // IRQ 81
    &default_irq_handler,              // IRQ 82
    &default_irq_handler,              // IRQ 83
    &default_irq_handler,              // IRQ 84
};
