#ifndef PROCESS_H
#define PROCESS_H


uint64_t* _createStack( uint64_t sp, void* fPtr);

void startFirstP();

void haltP();

void int20();


#endif