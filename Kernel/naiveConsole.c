// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "include/naiveConsole.h"
#include <keyboard.h>

#define GREY 7

static uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base);

static char buffer[64] = {'0'};
static uint8_t *const video = (uint8_t *) 0xB8000;
static uint8_t *currentVideo = (uint8_t *) 0xB8000;
static const uint32_t width = 80;
static const uint32_t height = 25 ;
//static uint8_t * previousVideo = video + (height-1)*width*2;

void ncPrint(const char *string) {
    int i;

    for (i = 0; string[i] != 0; i++)
        ncPrintChar(string[i]);
}

void ncPrintColor(const char *string, int color) {
    int i;

    for (i = 0; string[i] != 0; i++)
        ncPrintColorChar(string[i], color);
}

void ncPrintChar(char character) {
    ncScroll();
    if (character == '\n') {            // Enter
        ncNewline();
        return;
    } else if (character == '\b') {        // Backspace
        ncBackspace();
        return;
    } else if (character == '\t') {
        ncTab();
        return;

    }
    currentVideo[1] = GREY;
    *currentVideo = character;
    currentVideo += 2;
    currentVideo[1] = 0xf0;
}

void ncTab() {
    for (int i = 0; i < 5; i++) {
        currentVideo[1] = GREY;
        *currentVideo = ' ';
        currentVideo += 2;
        currentVideo[1] = 0xf0;
    }
}

void ncPrintColorChar(char character, int color) {
    ncScroll();
    *currentVideo++ = character;
    *currentVideo++ = color;
}

void ncScroll() {

    if (currentVideo - video == height * width * 2)
        ncNewline();
}

void ncNewline() {
    currentVideo[1] = GREY;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < 2 * width; j++)
            video[width * 2 * i + j] = video[width * 2 * (i + 1) + j];
    }
    for (int i = 0; i < 2 * width; i++) {
        if (i % 2 == 0)
            video[height * 2 * width - i] = ' ';
        else
            video[height * 2 * width - i] = GREY; //restauro color de celda
    }
    currentVideo = video + (height - 1) * width * 2;
    currentVideo[1] = 0xf0;
}


void ncPrintDec(uint64_t value) {
    ncPrintBase(value, 10);
}

void ncPrintHex(uint64_t value) {
    ncPrintBase(value, 16);
}


void ncPrintBase(uint64_t value, uint32_t base) {
    uintToBase(value, buffer, base);
    ncPrint(buffer);
}

void ncBackspace() {
    if ((int) (currentVideo - video) % (width * 2) == 0) return;
    currentVideo[1] = GREY;
    currentVideo -= 2;
    *currentVideo = ' ';
    currentVideo[1] = 0xf0;
}

void ncClear() {
    int i;
    for (i = 0; i < height * width; i++) {
        video[i * 2] = ' ';
        video[i * 2 + 1] = GREY;
    }
    currentVideo = video + (height - 1) * width * 2;
}

static uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base) {
    char *p = buffer;
    char *p1, *p2;
    uint32_t digits = 0;

    //Calculate characters for each digit
    do {
        uint32_t remainder = value % base;
        *p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
        digits++;
    } while (value /= base);

    // Terminate string in buffer.
    *p = 0;

    //Reverse string in buffer.
    p1 = buffer;
    p2 = p - 1;
    while (p1 < p2) {
        char tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }

    return digits;
}
