#include "terminal.hpp"
#include "../drivers/video/vga.hpp"

size_t strlen(const char* str)
{
    size_t len = 0;
    for (; str[len] != '\0'; ++len);
    return len;
}

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
