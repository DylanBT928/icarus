#ifndef GDT_HPP
#define GDT_HPP

#include <stdint.h>

extern "C"
{
    void set_gdt(uint16_t limit, const void* base);
    void reload_segments(uint16_t code_sel, uint16_t data_sel);
}

struct GDTEntrySrc
{
    uint32_t base;
    uint32_t limit;
    uint8_t access_byte;
    uint8_t flags;
};

class GDT
{
   public:
    static void init();

   private:
    static uint8_t s_table[24];
    static void encode(uint8_t* target, GDTEntrySrc src);
};

#endif
