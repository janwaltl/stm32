#pragma once

#include <chrono>
#include <cstdint>

void
init_sysclock();

std::chrono::nanoseconds
sysclock_period();

std::chrono::milliseconds
get_systime();
