#include "../drivers/video/vga.hpp"
#include "../tty/terminal.hpp"

extern "C" void kmain(void)
{
    terminalInitialize();

    terminalSetColor(vgaEntryColor(VGA_COLOR_LIGHT_GRAY, VGA_COLOR_BLACK));
    terminalWriteString("Welcome to ");

    terminalSetColor(vgaEntryColor(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK));
    terminalWriteString("Icarus");
}
