#ifndef TERMINAL_HPP
#define TERMINAL_HPP

#include <stddef.h>
#include <stdint.h>

void terminalInitialize(void);
void terminalSetColor(uint8_t color);
void terminalPutEntryAt(char c, uint8_t color, size_t x, size_t y);
void terminalPutChar(char c);
void terminalWrite(const char* data, size_t size);
void terminalWriteString(const char* data);

#endif
