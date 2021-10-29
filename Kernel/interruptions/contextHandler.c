#include "contextHandler.h"
#include <stdint.h>
#include "../include/naiveConsole.h"
#include "../memoryManager.h"
#include "interrupts.h"
#include "process.h"
#include "../pipes.h"
#include <stdarg.h>

//struct process{
//    char state;
//    uint64_t* rbp;
//    uint64_t* rsp;
//    int pid
//}typedef process;

extern void updateStack();
static PCB* currentProcess = NULL;
static PCB* firstP = NULL;
static char lastPID = 0;

void updateRSP(uint64_t* sp){
    currentProcess->rsp=sp;
}

void yield(){
    currentProcess->times = currentProcess->priority;
    int20();
    return;
}

void unblockShell(){
    PCB* aux=currentProcess;
    while(aux->pid!=0){
        aux=aux->next;
        //ncPrintDec(aux->pid);
    }
//    ncPrint("Desbloqueo shell\n");
    aux->state=READY;
}

int blockProcessPID(char pid){
    PCB * aux = firstP;
    while(aux!=NULL){
        if(aux->pid==pid){
            aux->state=BLOCKED;
            if(currentProcess==aux)
                int20();
            return 0;
        }
        aux = aux->next;
    }
    return -1;
}
int unblockProcessPID(char pid){
    PCB * aux = firstP;
    while(aux!=NULL){
        if(aux->pid==pid){
            aux->state=READY;
            return 0;
        }
        aux = aux->next;
    }
    return -1;
}

void blockProcess(){
    currentProcess->state=BLOCKED;
//    ncPrint("Bloqueo shell\n");
    //ncPrintDec(currentProcess->pid);
    //ncPrint("\n");
    
    int20();
    return;
}

void exit(){
//    ncPrint("\nEXITT\nnext:");
//    ncPrintHex(currentProcess->next);
//    ncPrint("\nnextPID:");
//    ncPrintHex(currentProcess->next->pid);
//    ncPrint("\nnextRSP:");
//    ncPrintHex(currentProcess->next->rsp);
//    ncPrint("\nprev (deberia ser 0): ");
//    ncPrintHex(currentProcess->prev);

    if (currentProcess->prev!=0){
        currentProcess->prev->next=currentProcess->next;
    }
    else{
        firstP=currentProcess->next;
    }
    if (currentProcess->next!=0){
        currentProcess->next->prev=currentProcess->prev;
        PCB * auxP = currentProcess->next;
        free(currentProcess->rbp);
        free(currentProcess);
        currentProcess=auxP;
    }
    else{
        free(currentProcess->rbp);
        free(currentProcess);
        currentProcess = firstP;
    }
//    ncPrint("prehandl\n");
    updateStack();
}

void handler() {
//    ncPrintChar('5');

    if(currentProcess->times == currentProcess->priority || currentProcess->state==BLOCKED){
        currentProcess->times=0;
        do{
            currentProcess = ((currentProcess->next==0)? firstP : currentProcess->next);
        }while(currentProcess->state==BLOCKED);
    }else{
        currentProcess->times++;
    }
}

void addProcessToList(PCB* newP){
    if(firstP==NULL){
        newP->next=0;
        newP->prev=0;
        firstP=newP;
        currentProcess = newP;

        //ncPrintChar('3');
        return;
    }
    newP->next=firstP;
    newP->prev=0;
    firstP=newP;
    return;
}

void processWrapper(uint64_t fPtr, int nArgs, va_list * vaList){

}

char newProcess(uint64_t fPtr, char priority, char * arg1, int arg2, char * arg3) {  // Los procesos pueden recibir tres argumentos mas. Son un char *, int e int.
//    ncPrint("En newProcess: \n");
//    ncPrint("buffer ");
//    ncPrint(arg1);
//    ncPrint("\nfd ");
//    ncPrintDec(arg2);
//    ncPrint("\nsemId ");
//    ncPrint(arg3);
//    ncPrintChar('\n');
    uint64_t *rbp = alloc(1024 * sizeof(uint64_t));
    PCB *newP = alloc(sizeof(PCB));
    newP->rbp=rbp;
    newP->pid = lastPID++;
    newP->prev=0;
    newP->rsp = createStackContext(((uint64_t) & rbp[1023]), fPtr, arg1, arg2, arg3);
    newP->priority=priority;
    newP->times=0;
    newP->state=READY;
    newP->next = NULL;
    newP->inputPipe = getCurrentPCB()->inputPipe;       //// Nota: Cerrar el stdin o stdout desde un proceso los cierra en todos los procesos.
    newP->outputPipe = getCurrentPCB()->outputPipe;
    addProcessToList(newP);
    return (newP->pid);
}

uint64_t * firstProcess(uint64_t fPtr){
    //ncPrintChar('1');
    uint64_t * rbp = alloc(1024*sizeof (uint64_t));
    PCB* first = alloc(sizeof (PCB));
    first->rbp=rbp;
    first->pid=lastPID++;
    first->rsp= createStackContext((uint64_t) &rbp[1023], fPtr, NULL, -1, -1);
//    ncPrintChar('\n');

//    ncPrintHex(rbp);
//    ncPrintChar('\n');
//    ncPrintHex(first->rsp);
//    ncPrintChar('\n');
//
//    ncPrintHex(fPtr);
    first->priority=MAX_PRIORITY;
    first->next=0;
    first->prev=0;
//    ncPrintChar('2');

    addProcessToList(first);
    // Set pipes
    initialPipes(first);
//    ncPrintChar('4');
    newProcess(&haltP, MAX_PRIORITY, NULL, NULL, NULL); //creo proceso halt


    startFirstP();
    ncPrintChar('F');   //aca no dberia llegar
    return first->rsp;
};

uint64_t * getCurrentSP(){
    return currentProcess->rsp;
}
char getCurrentPID(){
    return currentProcess->pid;
}

PCB * getCurrentPCB(){
    return currentProcess;
}

  /*  uint64_t* bp = alloc(1000 * sizeof(uint64_t));
    uint64_t * sp = _createStack((uint64_t)&sp[999], fPtr);
    PCB newP;
    newP.rsp = sp;
    newP.state=0;
    newP.pid=pCount++;
    rr[pCount++]=newP;
    pCount=pCount%256;
    return newP.rsp;*/


//void exit(int n){
//
//}

/*

 fork(){
    crearProceso(&hijo);
    padre: devuelve 0;
    hijo: devuelve index++;

 }
 hijo(){
 }


 syscallCrearProceso(new IP){
    pushstate-ip;
    push newIP;
    newProcess
 }
 */
