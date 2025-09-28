#ifndef VGA_HPP
#define VGA_HPP

#include <stddef.h>
#include <stdint.h>

// Hardware text mode color constants
enum vgaColor
{
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GRAY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15
};

static inline uint8_t vgaEntryColor(enum vgaColor fg, enum vgaColor bg)
{
    return fg | bg << 4;
}

static inline uint16_t vgaEntry(unsigned char uc, uint8_t color)
{
    return static_cast<uint16_t>(uc) | static_cast<uint16_t>(color << 8);
}

namespace VGA
{
inline constexpr size_t WIDTH = 80;
inline constexpr size_t HEIGHT = 25;
inline constexpr uintptr_t MEMORY = 0xB8000;
}  // namespace VGA

#endif
