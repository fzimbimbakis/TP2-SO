
#ifndef _CONTEXTHANDLER_H
#define _CONTEXTHANDLER_H

#include <stdint.h>
#define MAX_PRIORITY 2
#define SHELL 0

enum State {READY, BLOCKED,DEAD};

typedef struct PCB{
    uint32_t pid;
    enum State state;
    uint64_t* rsp;
    uint64_t* rbp;
    char priority; //TODO implementar las prioridades
    char times;
    struct PCB* next;
    struct PCB* prev;
}PCB;

/*typedef struct ProcessList{
    PCB * head;
    PCB* current;
    int size;
    int blocked;
}ProcessList;*/

void handler();
void yield();
void printProcesses();
int kill(uint32_t pid);
uint32_t getPid();
void killProcess(PCB* process);
int changePriority(uint32_t pid, char newPrio);
uint64_t * getCurrentSP();
PCB * getCurrentPCB();
void blockProcess();
blockProcessPID(uint32_t pid);
void unblockShell();
char newProcess(uint64_t fPtr, char priority);
void exit();
uint64_t * firstProcess(uint64_t fPtr);
extern uint64_t * createStackContext(uint64_t sp, uint64_t fPtr);
#endif