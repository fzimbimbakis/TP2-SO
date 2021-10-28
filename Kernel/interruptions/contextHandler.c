#include "contextHandler.h"
#include <stdint.h>
#include "../include/naiveConsole.h"
#include "../memoryManager.h"
#include "interrupts.h"
#include "process.h"

static PCB* currentProcess = NULL;
static PCB* firstP = NULL;
static uint32_t lastPID = 0;
static PCB* halt=NULL;

void printProcesses(){
    PCB* aux=firstP;
    while(aux != NULL){
        ncPrintDec(aux->pid);
        ncPrint("\n");
        aux=aux->next;
    }
}

void updateRSP(uint64_t* sp){
    currentProcess->rsp=sp;
}

uint32_t getPid(){
    return currentProcess->pid;
}

int changePriority(uint32_t pid, char newPrio){
    PCB* aux =firstP;
    while(aux!=NULL){
        if(aux->pid==pid){
            aux->priority=newPrio;
            return 0;
        }
        aux=aux->next;
    }
    return -1;
}

void unblockShell(){
    PCB* aux=firstP;
    while(aux->pid!= SHELL){
        aux=aux->next;
        //ncPrintDec(aux->pid);
    }
    //ncPrint("UNBLOCK\n");
    aux->state=READY;
    if(currentProcess==halt){
        currentProcess=aux;
        updateStack();
    }
}

void yield(){
    //ncPrint("YIELD\n");
    currentProcess->times=currentProcess->priority;
    int20();
}

void blockProcess(){
    currentProcess->state=BLOCKED;
    //ncPrint("BLOCK\n");
    int20();
    return;
}
int unblockProcessPID(uint32_t pid){
    PCB * aux = firstP;
    while(aux!=NULL){
        if(aux->pid==pid){
            aux->state=READY;
            if(currentProcess==halt){
                currentProcess=aux;
                updateStack();
            }

            return 0;
        }
        aux = aux->next;
    }
    return -1;
}

int blockProcessPID(uint32_t pid){
    //ncPrint("BLOCK\n");
    PCB* aux=firstP;
    while(aux!=NULL){
        //ncPrintDec(aux->pid);
        //ncPrint(" AUX\n");
        if(aux->pid==pid){
            aux->state=BLOCKED;
            if(currentProcess==aux)
                int20();

            return 0;
        }
        aux=aux->next;

    }
    
    return -1;
}

void exit(){

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
        currentProcess=firstP; //faltaria ver si es NULL??? el free lo deja en NULL??
    }
//    ncPrint("prehandl\n");
    updateStack();
}


int kill(uint32_t pid){
    //ncPrintDec(pid);
    //ncPrint(" KILL\n");
    

    if(currentProcess->pid==pid)
        exit();

    PCB* aux =firstP;
    while( aux!= NULL){
        if(aux->pid==pid){
            killProcess(aux);
            return 0;
        }
        aux=aux->next;
    }
    return -1;
}

void killProcess(PCB* process){
        if (process->prev!=0)
            process->prev->next=process->next;
        else
            firstP=process->next;


        if (process->next!=0)
            process->next->prev=process->prev;

        free(process->rbp);
        free(process);
}



void handler() {
//    ncPrintChar('5');
    if(currentProcess!=halt){
        uint32_t currentPid=currentProcess->pid;

        if(currentProcess->times == currentProcess->priority || currentProcess->state==BLOCKED){
            currentProcess->times=0;
            do{
                currentProcess = ((currentProcess->next==0)? firstP : currentProcess->next);

            }while(currentProcess->state==BLOCKED && currentProcess->pid!=currentPid);

            if(currentProcess->state==BLOCKED)//di toda la vuelta y no hay un proceso ready
                currentProcess=halt;

        }else{
            currentProcess->times++;
        }
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
    firstP->prev=newP;
    newP->prev=0;
    firstP=newP;
}

char newProcess(uint64_t fPtr, char priority) {
    uint64_t *rbp = alloc(1024 * sizeof(uint64_t));
    PCB *newP = alloc(sizeof(PCB));
    newP->rbp=rbp;
    newP->pid = lastPID++;
    newP->prev=0;
    newP->rsp = createStackContext(((uint64_t) & rbp[1023]), fPtr);
    newP->priority=priority;
    newP->times=0;
    newP->state=READY;
    newP->next = NULL;
    addProcessToList(newP);
    return (newP->pid);
}

uint64_t * firstProcess(uint64_t fPtr){ //deberia ser void????
    //ncPrintChar('1');
    newProcess(fPtr, MAX_PRIORITY);
    
    uint64_t * rbp = alloc(1024*sizeof (uint64_t));
    halt = alloc(sizeof (PCB));
    halt->rbp=rbp;
    halt->rsp= createStackContext((uint64_t) &rbp[1023], &haltP);
//    ncPrintChar('\n');

//    ncPrintHex(rbp);
//    ncPrintChar('\n');
//    ncPrintHex(first->rsp);
//    ncPrintChar('\n');
//
//    ncPrintHex(fPtr);
//    ncPrintChar('2');
    
//    ncPrintChar('4');
    //newProcess(&haltP, MAX_PRIORITY); //creo proceso halt
    startFirstP();
    ncPrintChar('F');   //aca no dberia llegar
    return 0;
};

uint64_t * getCurrentSP(){
    return currentProcess->rsp;
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
