#ifndef IO_HPP
#define IO_HPP

#include <stdint.h>

static inline void outb(uint16_t p, uint8_t v)
{
    asm volatile("outb %0,%1" ::"a"(v), "Nd"(p));
}

static inline uint8_t inb(uint16_t p)
{
    uint8_t v;
    asm volatile("inb %1,%0" : "=a"(v) : "Nd"(p));
    return v;
}

#endif
