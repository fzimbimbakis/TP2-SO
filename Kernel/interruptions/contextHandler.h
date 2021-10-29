
#ifndef _CONTEXTHANDLER_H
#define _CONTEXTHANDLER_H

#include <stdint.h>
#define READY 0
#define BLOCKED 1
#define DEAD 2
#define MAX_PRIORITY 2
#include "../pipes.h"
typedef struct PCB PCB;
struct PCB{
    char pid;
    char state;
    uint64_t* rsp;
    uint64_t* rbp;
    char priority;
    char times;
    struct pipe_t * inputPipe;     //// Agregar en el informe esta decision.
    struct pipe_t * outputPipe;
    struct PCB* next;
    struct PCB* prev;
};

void handler();
uint64_t * getCurrentSP();
PCB * getCurrentPCB();
char getCurrentPID();
int blockProcessPID(char pid);
int unblockProcessPID(char pid);
void yield();
void blockProcess();
void unblockShell();

char newProcess(uint64_t fPtr, char priority, char * arg1, int arg2, char * arg3);
void exit();
uint64_t * firstProcess(uint64_t fPtr);
extern uint64_t * createStackContext(uint64_t sp, uint64_t fPtr, char * arg1, int agr2, char * agr3);
#endif