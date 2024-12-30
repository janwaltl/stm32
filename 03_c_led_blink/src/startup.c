#include <stddef.h>
#include <stdint.h>

// Set by linker script
extern unsigned char _bdata;      // NOLINT
extern unsigned char _edata;      // NOLINT
extern unsigned char _bbss;       // NOLINT
extern unsigned char _ebss;       // NOLINT
extern unsigned char _ldbdata;    // NOLINT
extern unsigned char _binit_data; // NOLINT
extern unsigned char _einit_data; // NOLINT

extern int
main(void);
extern void
halt(void);

// Call all compiler-generated init functions.
static void
startup_init_data_init(void) {
    typedef void (*init_fun)(void);

    init_fun *p = (init_fun *)(&_binit_data);
    while (p < (init_fun *)&_einit_data) {
        (*p++)();
    }
}

// copy .data section from FLASH to RAM
static void
startup_data_init(void) {
    volatile unsigned char *p = &_bdata;
    volatile unsigned char *end = &_edata;
    volatile unsigned char *from = &_ldbdata;

    while (p < end) {
        *p++ = *from++;
    }
}

// zero-out .bss section in RAM
static void
startup_bss_init(void) {
    volatile unsigned char *p = &_bbss;

    while (p < &_ebss) {
        *p++ = 0;
    }
}

// Entry point
void
reset_irq_handler(void) {
    startup_data_init();
    startup_bss_init();
    startup_init_data_init();

    main();
    halt();
}
