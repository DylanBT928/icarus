#include "pic.hpp"
#include "../../core/io.hpp"

static inline void io_wait()
{
    asm volatile("outb %0,%1" ::"a"((uint8_t)0), "Nd"((uint16_t)0x80));
}

void pic_remap(uint8_t off1, uint8_t off2)
{
    uint8_t a1 = inb(0x21);
    uint8_t a2 = inb(0xA1);

    outb(0x20, 0x11);
    io_wait();
    outb(0xA0, 0x11);
    io_wait();
    outb(0x21, off1);
    io_wait();
    outb(0x21, off2);
    io_wait();
    outb(0x21, 0x04);
    io_wait();
    outb(0xA1, 0x02);
    io_wait();
    outb(0x21, 0x01);
    io_wait();
    outb(0xA1, 0x01);
    io_wait();

    outb(0x21, a1);
    outb(0xA1, a2);
}

void pic_set_masks(uint8_t parent_mask, uint8_t child_mask)
{
    outb(0x21, parent_mask);
    outb(0xA1, child_mask);
}

void pic_eoi(uint8_t irq)
{
    if (irq >= 8)
    {
        asm volatile("outb %0,%1" ::"a"((uint8_t)0x20), "Nd"((uint16_t)0xA0));
    }

    asm volatile("outb %0,%1" ::"a"((uint8_t)0x20), "Nd"((uint16_t)0x20));
}
