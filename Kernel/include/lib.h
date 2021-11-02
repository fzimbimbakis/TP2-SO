#ifndef LIB_H
#define LIB_H

#include <stdint.h>
#define MAX_NRO_BUFFER 32
void *memset(void *destination, int32_t character, uint64_t length);

void *memcpy(void *destination, const void *source, uint64_t length);

char *numToStrK(unsigned long int num, int base, char * index);

int myStrcmp(const char *a, const char *b);

char *cpuVendor(char *result);

int myStrlen(char *string);

void myStrcpy(char *from, char *to);

#endif