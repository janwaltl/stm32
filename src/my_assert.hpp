#pragma once

#define my_assert(arg, msg)                                                    \
    do {                                                                       \
        (void)static_cast<const char *>(msg);                                  \
        (void)static_cast<bool>(arg);                                          \
    } while (false);
