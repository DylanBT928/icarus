#include "isr_common.hpp"

extern "C"
{
    void isr_common(uint16_t vec, uint64_t err)
    {
        auto phex = [](uint64_t x)
        {
            char buf[19];
            buf[18] = 0;

            for (int i = 17; i >= 2; --i)
            {
                int d = x & 0xF;
                buf[i] = "0123456789abcdef"[d];
                x >>= 4;
            }

            buf[0] = '0';
            buf[1] = 'x';
            serial_print(buf);
        };

        auto pdec = [](uint64_t v)
        {
            if (!v)
            {
                serial_putc('0');
                return;
            }

            char t[32];
            int i = 0;

            while (v)
            {
                t[i++] = '0' + (v % 10);
                v /= 10;
            }

            while (i--)
            {
                serial_putc(t[i]);
            }
        };

        if (vec == 0x0E)
        {
            uint64_t cr2;
            asm volatile("mov %%cr2, %0" : "=r"(cr2));
            serial_print("EXC #PF vec14 err=");
            phex(err);
            serial_print(" cr2=");
            phex(cr2);
            serial_print("\n");
        }
        else
        {
            serial_print("EXC vec=");
            pdec(vec);
            serial_print(" err=");
            phex(err);
            serial_print("\n");
        }
    }
}
