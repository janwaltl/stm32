#include <stddef.h>

void
init_serial(void);

void
serial_send_char(char c);

void
serial_send_str(const char *msg);

size_t
serial_receive(char *dst, size_t n);
