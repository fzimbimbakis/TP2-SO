// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "contextHandler.h"
#include <stdint.h>
#include "../include/naiveConsole.h"
#include "../include/memoryManager.h"
#include "interrupts.h"
#include "process.h"
#include "time.h"
#include "../pipes.h"
#include <stdarg.h>

static PCB *currentProcess = NULL;
static PCB *firstP = NULL;
static uint32_t lastPID = 0;
static PCB *halt = NULL;

void printProcesses() { 
    PCB *aux = firstP;
    while (aux != NULL) {
        ncPrint(aux->name);
        ncPrint("  ");

        ncPrint("Pid:");
        ncPrintDec(aux->pid);
        ncPrint("  ");

        ncPrint("Prio:");
        ncPrintDec(aux->priority);
        ncPrint("   ");

        ncPrint("Background:");
        if (aux->isBackground) {
            ncPrint("Yes");
        } else ncPrint("No");
        ncPrint("  ");


        ncPrint("Rsp:");
        ncPrintHex((uint64_t) aux->rsp);
        ncPrint("  ");

        ncPrint("Rbp:");
        ncPrintHex((uint64_t) aux->rbp);
        ncPrint("  ");

        if (aux->state == BLOCKED)
            ncPrint("Blocked");
        else
            ncPrint("Ready");
        ncPrint("\n");
        aux = aux->next;
    }

}

//
void updateRSP(uint64_t *sp) {
    currentProcess->rsp = sp;
}

uint32_t getPid() {
    return currentProcess->pid;
}

int changePriority(uint32_t pid, char newPrio) {
    if (newPrio>MAX_PRIORITY){
        ncPrint("Max Priority is : ");
        ncPrintDec(MAX_PRIORITY);
        ncPrintChar('\n');
        return -2;
    }
    PCB *aux = firstP;
    while (aux != NULL) {
        if (aux->pid == pid) {
            aux->priority = newPrio;
            return 0;
        }
        aux = aux->next;
    }
    return -1;
}

void yield() {
    currentProcess->times = currentProcess->priority;
    int20();
    return;
}


int unblockProcessPID(uint32_t pid) {
    PCB *aux = firstP;
    while (aux != NULL) {
        if (aux->pid == pid) {
            aux->state = READY;
            if (currentProcess == halt) {
                currentProcess = aux;
                updateStack();
            }

            return 0;
        }
        aux = aux->next;
    }
    return -1;
}

int blockProcessPID(uint32_t pid) {

    PCB *aux = firstP;
    while (aux != NULL) {

        if (aux->pid == pid) {
            aux->state = BLOCKED;
            if (currentProcess == aux)
                int20();

            return 0;
        }
        aux = aux->next;

    }

    return -1;
}

void blockProcess() {
    currentProcess->state = BLOCKED;
    int20();
    return;
}

void exit() {

    if (currentProcess->prev != 0) {
        currentProcess->prev->next = currentProcess->next;
    } else {
        firstP = currentProcess->next;
    }
    if (currentProcess->next != 0) {
        currentProcess->next->prev = currentProcess->prev;
        PCB *auxP = currentProcess->next;
        free(currentProcess->rbp);
        free(currentProcess);
        currentProcess = auxP;
    } else {

        free(currentProcess->rbp);
        free(currentProcess);
        currentProcess = firstP;
    }
    updateStack();
}


int kill(uint32_t pid) {
    if (currentProcess->pid == pid)
        exit();

    PCB *aux = firstP;
    while (aux != NULL) {
        if (aux->pid == pid) {
            killProcess(aux);
            return 0;
        }
        aux = aux->next;
    }
    return -1;
}

void killProcess(PCB *process) {
    if (process->prev != 0)
        process->prev->next = process->next;
    else
        firstP = process->next;


    if (process->next != 0)
        process->next->prev = process->prev;

    free(process->rbp);
    free(process);
}


void handler() {
    timer_handler();
    if (currentProcess != halt) {
        uint32_t currentPid = currentProcess->pid;

        if (currentProcess->times == currentProcess->priority || currentProcess->state == BLOCKED) {
            currentProcess->times = 0;
            do {
                currentProcess = ((currentProcess->next == 0) ? firstP : currentProcess->next);

            } while (currentProcess->state == BLOCKED && currentProcess->pid != currentPid);

            if (currentProcess->state == BLOCKED)//di toda la vuelta y no hay un proceso ready
                currentProcess = halt;

        } else {
            currentProcess->times++;
        }
    }
}


void addProcessToList(PCB *newP) {
    if (firstP == NULL) {
        newP->next = 0;
        newP->prev = 0;
        firstP = newP;
        currentProcess = newP;
        return;
    }

    newP->next = firstP;
    firstP->prev = newP;
    newP->prev = 0;
    firstP = newP;
}


char newProcess(uint64_t fPtr, char isBackgroud, char *arg1, int arg2,
                char *arg3) {  // Los procesos pueden recibir tres argumentos mas. Son un char *, int e int.
    uint64_t *rbp = alloc(1024 * sizeof(uint64_t));
    PCB *newP = alloc(sizeof(PCB));
    newP->rbp = rbp;
    newP->pid = lastPID++;
    newP->prev = 0;
    newP->rsp = createStackContext(((uint64_t) & rbp[1023]), fPtr, arg1, arg2, arg3);
    newP->priority = 0;
    myStrcpy(newP->name, arg1);
    newP->isBackground = isBackgroud;
    newP->times = 0;
    newP->state = READY;
    newP->next = NULL;
    newP->inputPipe = getCurrentPCB()->inputPipe;
    newP->outputPipe = getCurrentPCB()->outputPipe;
    addProcessToList(newP);
    return (newP->pid);
}

uint64_t *firstProcess(uint64_t fPtr) { //deberia ser void????

    uint64_t *rbp = alloc(1024 * sizeof(uint64_t));
    PCB *first = alloc(sizeof(PCB));
    first->rbp = rbp;
    first->pid = lastPID++;
    first->rsp = createStackContext((uint64_t) & rbp[1023], fPtr, NULL, -1, NULL);

    first->priority = MAX_PRIORITY;
    first->next = 0;
    first->prev = 0;
    myStrcpy(first->name, "Shell");
    initialPipes(first);
    uint64_t *rbpHalt = alloc(1024 * sizeof(uint64_t));
    halt = alloc(sizeof(PCB));
    halt->rbp = rbp;
    halt->rsp = createStackContext((uint64_t) & rbpHalt[1023], (uint64_t) & haltP, NULL, -1, NULL);
    halt->inputPipe = first->inputPipe;
    halt->outputPipe = first->outputPipe;
    halt->priority = 0;
    halt->next = 0;
    halt->prev = 0;

    addProcessToList(first);
    startFirstP();
    return 0;
};

uint64_t *getCurrentSP() {
    return currentProcess->rsp;
}


PCB *getCurrentPCB() {
    return currentProcess;
}

