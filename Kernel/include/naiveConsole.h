#ifndef NAIVE_CONSOLE_H
#define NAIVE_CONSOLE_H

#include <stdint.h>

void ncPrint(const char *string);

void ncPrintChar(char character);

void ncNewline();

void ncPrintDec(uint64_t value);

void ncPrintHex(uint64_t value);

void ncPrintBase(uint64_t value, uint32_t base);

void ncScroll();

void ncClear();

void ncPrintColor(const char *string, int color);

void ncPrintColorChar(char character, int color);

void ncBackspace();

void ncTab();


#endif