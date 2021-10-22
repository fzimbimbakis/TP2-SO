
#ifndef _CONTEXTHANDLER_H
#define _CONTEXTHANDLER_H

#include <stdint.h>
#define READY 0
#define BLOCKED 1
#define DEAD 2
#define MAX_PRIORITY 2

typedef struct PCB{
    char pid;
    char state;
    uint64_t* rsp;
    uint64_t* rbp;
    char priority; //TODO implementar las prioridades
    char times;
    struct PCB* next;
    struct PCB* prev;
}PCB;

void handler();
uint64_t * getCurrentSP();
PCB * getCurrentPCB();
void blockProcess();
void unblockShell();
char newProcess(uint64_t fPtr, char priority);
void exit();
uint64_t * firstProcess(uint64_t fPtr);
extern uint64_t * createStackContext(uint64_t sp, uint64_t fPtr);
#endif