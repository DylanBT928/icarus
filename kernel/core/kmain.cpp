#include "../drivers/video/vga.hpp"
#include "../tty/terminal.hpp"

extern "C" void kmain(void)
{
    terminalInitialize();

    terminalSetColor(vgaEntryColor(VGA_COLOR_LIGHT_GRAY, VGA_COLOR_BLACK));
    terminalWriteString("Welcome to ");

    terminalSetColor(vgaEntryColor(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK));
    terminalWriteString("Icarus");

    terminalSetColor(vgaEntryColor(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK));
    terminalWriteString("\n\nHello World\n");

    terminalSetColor(vgaEntryColor(VGA_COLOR_GREEN, VGA_COLOR_BLACK));
    terminalWriteString("\n1");
    terminalWriteString("\n2");
    terminalWriteString("\n3");
    terminalWriteString("\n4");
    terminalWriteString("\n5");
    terminalWriteString("\n6");
    terminalWriteString("\n7");
    terminalWriteString("\n8");
    terminalWriteString("\n9");
    terminalWriteString("\n10");
    terminalWriteString("\n11");
    terminalWriteString("\n12");
    terminalWriteString("\n13");
    terminalWriteString("\n14");
    terminalWriteString("\n15");
    terminalWriteString("\n16");
    terminalWriteString("\n17");
    terminalWriteString("\n18");
    terminalWriteString("\n19");
    terminalWriteString("\n20");
    terminalWriteString("\n21");
    terminalWriteString("\n22");
    terminalWriteString("\n23");
    terminalWriteString("\n24");
    terminalWriteString("\n25");
}
