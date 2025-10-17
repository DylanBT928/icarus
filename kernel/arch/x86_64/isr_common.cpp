#include "isr_common.hpp"
#include "pic.hpp"
#include "../../core/io.hpp"

static volatile uint64_t g_ticks = 0;

extern "C"
{
    void isr_common(uint16_t vec, uint64_t err)
    {
        if (vec == 0x20)
        {
            *(volatile uint64_t*)&g_ticks = g_ticks + 1;

            if ((g_ticks % 100) == 0)
            {
                serial_print("tick\n");
            }

            pic_eoi(0);
            return;
        }

        if (vec == 0x21)
        {
            uint8_t scancode = inb(0x60);
            serial_print("keyboard scancode ");

            char buf[6] = "0x00\n";
            const char* hex = "0123456789abcdef";

            buf[2] = hex[(scancode >> 4) & 0xF];
            buf[3] = hex[scancode & 0xF];

            serial_print(buf);
            pic_eoi(1);
            return;
        }

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
