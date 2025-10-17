#include "pit.hpp"
#include "../../core/io.hpp"

void pit_init(uint32_t hz)
{
    if (!hz)
    {
        hz = 100;
    }

    uint32_t divisor = 1193182u / hz;

    outb(0x43, 0x36);
    outb(0x40, divisor & 0xFF);
    outb(0x40, (divisor >> 8) & 0xFF);
}
