#ifndef IDT_HPP
#define IDT_HPP

#include <stdint.h>

extern "C"
{
    void set_idt(uint16_t limit, const void* base);
}

struct IDTGateSrc
{
    void (*handler)();
    uint16_t selector;
    uint8_t type_attr;
    uint8_t ist;
};

class IDT
{
   public:
    static void init();

   private:
    static void encode(uint8_t* t, IDTGateSrc s);
    static uint8_t s_table[16 * 256];
};

#endif
