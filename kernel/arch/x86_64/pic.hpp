#ifndef PIC_HPP
#define PIC_HPP

#include <stdint.h>

void pic_remap(uint8_t off1 = 0x20, uint8_t off2 = 0x28);
void pic_set_masks(uint8_t parent_mask, uint8_t child_mask);
void pic_eoi(uint8_t irq);

#endif
