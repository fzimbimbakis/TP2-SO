#include "MM.h"
#define BEGIN_MEM 0x60000
#define END_MEM 0x9FFFF

void * current;

void * init(){
	current = (void*)BEGIN_MEM;
}

void * alloc(size_t size){

	if(current+size > END_MEM)
		return NULL;

	void * aux= current;

	current+=size;

	return aux;
}