
#ifndef _CONTEXTHANDLER_H
#define _CONTEXTHANDLER_H

#include <stdint.h>
#include "../pipes.h"

#define MAX_PRIORITY 2
#define SHELL 0
#define NAME_LENGTH 20

enum State {
    READY, BLOCKED, DEAD
};

typedef struct PCB {
    uint32_t pid;
    char isBackground;
    char name[NAME_LENGTH];
    enum State state;
    uint64_t *rsp;
    uint64_t *rbp;
    char priority;
    char times;
    struct pipe_t *inputPipe;     //// Agregar en el informe esta decision.
    struct pipe_t *outputPipe;
    struct PCB *next;
    struct PCB *prev;
} PCB;

void handler();

void yield();

void printProcesses();

int kill(uint32_t pid);

uint32_t getPid();

void killProcess(PCB *process);

int changePriority(uint32_t pid, char newPrio);

uint64_t *getCurrentSP();

PCB *getCurrentPCB();

void blockProcess();

int blockProcessPID(uint32_t pid);

int unblockProcessPID(uint32_t pid);
//void unblockShell();

char newProcess(uint64_t fPtr, char priority, char *arg1, int arg2, char *arg3);

void exit();

uint64_t *firstProcess(uint64_t fPtr);

extern uint64_t *createStackContext(uint64_t sp, uint64_t fPtr, char *arg1, int agr2, char *agr3);

#endif