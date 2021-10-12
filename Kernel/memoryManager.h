#ifndef MEM_MAN_H
#define MEM_MAN_H




#include<stddef.h>
#define BEGIN_MEM 0x600000
#define END_MEM 0x700000
void * alloc(unsigned nbytes);
void free(void * block);
void init();

#endif