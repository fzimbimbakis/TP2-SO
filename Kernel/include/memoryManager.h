#ifndef MEM_MAN_H
#define MEM_MAN_H



#include <stdint.h>
#include <unistd.h>
#include<stddef.h>

#define BEGIN_MEM (unsigned)0x600000
#define END_MEM (unsigned)0xFFFFFF
void * alloc(unsigned nbytes);
void free(void * block);
unsigned * memoryInfoMM();
void init();

#endif