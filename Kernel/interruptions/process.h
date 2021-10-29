#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>


uint64_t* _createStack( uint64_t sp, void* fPtr);

void startFirstP();

void haltP();

void int20();

void updateStack();


#endif