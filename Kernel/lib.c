// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <stdint.h>
#include "include/lib.h"
#include "include/memoryManager.h"
void *memset(void *destination, int32_t c, uint64_t length) {
    uint8_t chr = (uint8_t) c;
    char *dst = (char *) destination;

    while (length--)
        dst[length] = chr;

    return destination;
}
static char hexArrayK[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
char *numToStrK(uint64_t num, int base, char * index) {
    int aux = num % base;
    int i = MAX_NRO_BUFFER;
    char * buffer = alloc(i--);
    buffer[i--]=0;
    if (num == 0)
        buffer[i--] = '0';
    while (i >= 0 && num > 0) {
        aux = num % base;
        buffer[i] = hexArrayK[aux];
        i--;
        num /= base;
    }
    *index = i+1;
    return buffer;
}
void *memcpy(void *destination, const void *source, uint64_t length) {
    /*
    * memcpy does not support overlapping buffers, so always do it
    * forwards. (Don't change this without adjusting memmove.)
    *
    * For speedy copying, optimize the common case where both pointers
    * and the length are word-aligned, and copy word-at-a-time instead
    * of byte-at-a-time. Otherwise, copy by bytes.
    *
    * The alignment logic below should be portable. We rely on
    * the compiler to be reasonably intelligent about optimizing
    * the divides and modulos out. Fortunately, it is.
    */
    uint64_t i;

    if ((uint64_t) destination % sizeof(uint32_t) == 0 &&
        (uint64_t) source % sizeof(uint32_t) == 0 &&
        length % sizeof(uint32_t) == 0) {
        uint32_t *d = (uint32_t *) destination;
        const uint32_t *s = (const uint32_t *) source;

        for (i = 0; i < length / sizeof(uint32_t); i++)
            d[i] = s[i];
    } else {
        uint8_t *d = (uint8_t *) destination;
        const uint8_t *s = (const uint8_t *) source;

        for (i = 0; i < length; i++)
            d[i] = s[i];
    }

    return destination;
}

int myStrcmp(const char *a, const char *b) {
    int i = 0;
    while (a[i] != 0 && b[i] != 0) {
        if (a[i] != b[i])
            return 0;
        i++;
    }
    return a[i] == 0 && b[i] == 0;
}

int myStrlen(char *string) {
    int i = 0;
    while (string[i] != 0) {
        i++;
    }
    return i;
}

void myStrcpy(char *to, char *from) {
    int i = 0;
    while (from[i] != 0) {
        to[i] = from[i];
        i++;
    }
    to[i] = 0;
}
