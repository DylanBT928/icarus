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

size_t strlen(const char* str)
{
    size_t len = 0;
    for (; str[len] != '\0'; ++len);
    return len;
}

namespace VGA
{
inline constexpr size_t WIDTH = 80;
inline constexpr size_t HEIGHT = 25;
inline constexpr uintptr_t MEMORY = 0xB8000;
}  // namespace VGA

size_t terminalRow;
size_t terminalColumn;
uint8_t terminalColor;
static volatile uint16_t* const terminalBuffer = reinterpret_cast<volatile uint16_t*>(VGA::MEMORY);

void terminalInitialize(void)
{
    terminalRow = 0;
    terminalColumn = 0;
    terminalColor = vgaEntryColor(VGA_COLOR_LIGHT_GRAY, VGA_COLOR_BLACK);

    for (size_t y = 0; y < VGA::HEIGHT; ++y)
    {
        for (size_t x = 0; x < VGA::WIDTH; ++x)
        {
            const size_t i = y * VGA::WIDTH + x;
            terminalBuffer[i] = vgaEntry(' ', terminalColor);
        }
    }
}

void terminalSetColor(uint8_t color)
{
    terminalColor = color;
}

void terminalPutEntryAt(char c, uint8_t color, size_t x, size_t y)
{
    const size_t i = y * VGA::WIDTH + x;
    terminalBuffer[i] = vgaEntry(c, color);
}

void terminalPutChar(char c)
{
    terminalPutEntryAt(c, terminalColor, terminalColumn, terminalRow);

    if (++terminalColumn == VGA::WIDTH)
    {
        terminalColumn = 0;

        if (++terminalRow == VGA::HEIGHT)
        {
            terminalRow = 0;
        }
    }
}

void terminalWrite(const char* data, size_t size)
{
    for (size_t i = 0; i < size; ++i)
    {
        terminalPutChar(data[i]);
    }
}

void terminalWriteString(const char* data)
{
    terminalWrite(data, strlen(data));
}

extern "C" void kmain(void)
{
    terminalInitialize();
    terminalWriteString("\"Hello world\" - Icarus");
}
