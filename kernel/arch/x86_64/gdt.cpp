#include "gdt.hpp"

void GDT::encode(uint8_t* target, GDTEntrySrc src)
{
    if (src.limit > 0xFFFFF)
    {
        // GDT cannot encode limits larger than 0xFFFFF
        for (;;)
        {
            asm volatile("cli; hlt");
        }
    }

    // Encode the limit
    target[0] = src.limit & 0xFF;
    target[1] = (src.limit >> 8) & 0xFF;
    target[6] = (src.limit >> 16) & 0x0F;

    // Encode the base
    target[2] = src.base & 0xFF;
    target[3] = (src.base >> 8) & 0xFF;
    target[4] = (src.base >> 16) & 0xFF;
    target[7] = (src.base >> 24) & 0xFF;

    // Encode the access byte
    target[5] = src.access_byte;

    // Encode the flags
    target[6] |= (src.flags << 4);
}

alignas(16) uint8_t GDT::s_table[24];

void GDT::init()
{
    for (int i = 0; i < 8; ++i)
    {
        s_table[i] = 0;
    }

    // Kernel code
    GDTEntrySrc code{
        .base = 0,
        .limit = 0,
        .access_byte = 0x9A,
        .flags = 0xA
    };

    encode(&s_table[8], code);

    // Kernel data
    GDTEntrySrc data{
        .base = 0,
        .limit = 0,
        .access_byte = 0x92,
        .flags = 0x8
    };

    encode(&s_table[16], data);

    const uint16_t limit = sizeof(s_table) - 1;
    set_gdt(limit, &s_table[0]);

    constexpr uint16_t KCODE = 0x08;
    constexpr uint16_t KDATA = 0x10;

    reload_segments(KCODE, KDATA);
}
