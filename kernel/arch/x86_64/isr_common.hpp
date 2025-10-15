#ifndef ISR_COMMON_HPP
#define ISR_COMMON_HPP

#include "serial.hpp"

#include <stdint.h>

extern "C"
{
    void isr_common(uint16_t vec, uint64_t err);
}

#endif
