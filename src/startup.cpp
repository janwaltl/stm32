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
DEFAULT void
systick_irq_handler(void);
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
};
