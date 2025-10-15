#include "idt.hpp"

extern "C"
{
    void isr_stub_0x00();
    void isr_stub_0x01();
    void isr_stub_0x02();
    void isr_stub_0x03();
    void isr_stub_0x04();
    void isr_stub_0x05();
    void isr_stub_0x06();
    void isr_stub_0x07();
    void isr_stub_0x08();
    void isr_stub_0x09();
    void isr_stub_0x0A();
    void isr_stub_0x0B();
    void isr_stub_0x0C();
    void isr_stub_0x0D();
    void isr_stub_0x0E();
    void isr_stub_0x10();
    void isr_stub_0x11();
    void isr_stub_0x12();
    void isr_stub_0x13();
    void isr_stub_0x14();
    void isr_stub_0x15();
}

void IDT::encode(uint8_t* t, IDTGateSrc s)
{
    uint64_t addr = (uint64_t)s.handler;

    // offset [0:15]
    t[0] = (uint8_t)(addr & 0xFF);
    t[1] = (uint8_t)((addr >> 8) & 0xFF);

    // selector
    t[2] = (uint8_t)(s.selector & 0xFF);
    t[3] = (uint8_t)((s.selector >> 8) & 0xFF);

    // ist (bits 0..2), rest 0
    t[4] = (uint8_t)(s.ist & 0x7);

    // type_attr
    t[5] = s.type_attr;

    // offset [16:31]
    t[6] = (uint8_t)((addr >> 16) & 0xFF);
    t[7] = (uint8_t)((addr >> 24) & 0xFF);

    // offset [32:63]
    t[8] = (uint8_t)((addr >> 32) & 0xFF);
    t[9] = (uint8_t)((addr >> 40) & 0xFF);
    t[10] = (uint8_t)((addr >> 48) & 0xFF);
    t[11] = (uint8_t)((addr >> 56) & 0xFF);

    // zero
    t[12] = t[13] = t[14] = t[15] = 0;
}

alignas(16) uint8_t IDT::s_table[16 * 256];

void IDT::init()
{
    for (int i = 0; i < 16 * 256; ++i)
    {
        s_table[i] = 0;
    }

    constexpr uint16_t KCODE = 0x08;
    constexpr uint8_t INTG = 0x8E;  // interrupt gate
    constexpr uint8_t TRAP = 0x8F;  // trap gate

    auto set = [](int vec, void (*h)(), uint8_t* table, uint16_t sel, uint8_t type_attr, uint8_t ist = 0)
    {
        IDTGateSrc src{
            .handler = h,
            .selector = sel,
            .type_attr = type_attr,
            .ist = ist
        };
        IDT::encode(&table[vec * 16], src);
    };

    // Exceptions
    set(0x00, isr_stub_0x00, s_table, KCODE, INTG);  // #DE
    set(0x01, isr_stub_0x01, s_table, KCODE, TRAP);  // #DB
    set(0x02, isr_stub_0x02, s_table, KCODE, INTG);  // NMI
    set(0x03, isr_stub_0x03, s_table, KCODE, TRAP);  // #BP (int3)
    set(0x04, isr_stub_0x04, s_table, KCODE, TRAP);  // #OF
    set(0x05, isr_stub_0x05, s_table, KCODE, INTG);  // #BR
    set(0x06, isr_stub_0x06, s_table, KCODE, INTG);  // #UD (ud2)
    set(0x07, isr_stub_0x07, s_table, KCODE, INTG);  // #NM
    set(0x08, isr_stub_0x08, s_table, KCODE, INTG);  // #DF (err=0)
    set(0x09, isr_stub_0x09, s_table, KCODE, INTG);  // reserved
    set(0x0A, isr_stub_0x0A, s_table, KCODE, INTG);  // #TS (err)
    set(0x0B, isr_stub_0x0B, s_table, KCODE, INTG);  // #NP (err)
    set(0x0C, isr_stub_0x0C, s_table, KCODE, INTG);  // #SS (err)
    set(0x0D, isr_stub_0x0D, s_table, KCODE, INTG);  // #GP (err)
    set(0x0E, isr_stub_0x0E, s_table, KCODE, INTG);  // #PF (err)
    set(0x10, isr_stub_0x10, s_table, KCODE, INTG);  // #MF
    set(0x11, isr_stub_0x11, s_table, KCODE, INTG);  // #AC (err=0)
    set(0x12, isr_stub_0x12, s_table, KCODE, INTG);  // #MC
    set(0x13, isr_stub_0x13, s_table, KCODE, INTG);  // #XM
    set(0x14, isr_stub_0x14, s_table, KCODE, INTG);  // #VE
    set(0x15, isr_stub_0x15, s_table, KCODE, INTG);  // #CP (err)

    const uint16_t limit = sizeof(s_table) - 1;
    set_idt(limit, &s_table[0]);
}
