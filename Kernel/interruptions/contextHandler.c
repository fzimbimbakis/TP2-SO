#include "contextHandler.h"
#include <stdint.h>
#include "../include/naiveConsole.h"
#include "../memoryManager.h"
#include "interrupts.h"
#include "process.h"
#include "time.h"
#include "../pipes.h"
#include <stdarg.h>

static PCB* currentProcess = NULL;
static PCB* firstP = NULL;
static uint32_t lastPID = 0;
static PCB* halt=NULL;

void printProcesses(){ //TODO: agregar nombre a los procesos y background/foreground
    PCB* aux=firstP;
    while(aux != NULL){
        ncPrint("PID: ");
        ncPrintDec(aux->pid);
        ncPrint("\t");
        ncPrint("Priority: ");
        ncPrintDec(aux->priority);
        ncPrint("\t");
        ncPrint("RSP: ");
        ncPrintDec(aux->rsp);
        ncPrint("\t");
        ncPrint("RBP: ");
        ncPrintDec(aux->rbp);
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
    currentProcess->times = currentProcess->priority;
    int20();
    return;
}

//void unblockShell(){
//    PCB* aux=currentProcess;
//    while(aux->pid!=0){
//        aux=aux->next;
//        //ncPrintDec(aux->pid);
//    }
////    ncPrint("Desbloqueo shell\n");
//    aux->state=READY;
//}

//int blockProcessPID(char pid){
//    PCB * aux = firstP;
//    while(aux!=NULL){
//        if(aux->pid==pid){
//            aux->state=BLOCKED;
//            if(currentProcess==aux)
//                int20();
//            return 0;
//        }
//        aux = aux->next;
//    }
//    return -1;
//}
//int unblockProcessPID(char pid){
//    PCB * aux = firstP;
//    while(aux!=NULL){
//        if(aux->pid==pid){
//            aux->state=READY;
//            return 0;
//        }
//        aux = aux->next;
//    }
//    return -1;
//}
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

void blockProcess(){
    currentProcess->state=BLOCKED;
//    ncPrint("Bloqueo shell\n");
    //ncPrintDec(currentProcess->pid);
    //ncPrint("\n");
    
    int20();
    return;
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
        currentProcess=firstP;
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
    timer_handler();
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



char newProcess(uint64_t fPtr, char priority, char * arg1, int arg2, char * arg3) {  // Los procesos pueden recibir tres argumentos mas. Son un char *, int e int.
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

uint64_t * firstProcess(uint64_t fPtr){ //deberia ser void????
    //ncPrintChar('1');
    uint64_t * rbp = alloc(1024*sizeof (uint64_t));
    PCB* first = alloc(sizeof (PCB));
    first->rbp=rbp;
    first->pid=lastPID++;
    first->rsp= createStackContext((uint64_t) &rbp[1023], fPtr, NULL, -1, NULL);

    first->priority=MAX_PRIORITY;
    first->next=0;
    first->prev=0;

    initialPipes(first);
    uint64_t * rbp = alloc(1024*sizeof (uint64_t));
    halt = alloc(sizeof (PCB));
    halt->rbp=rbp;
    halt->rsp= createStackContext((uint64_t) &rbp[1023], &haltP, NULL, -1, NULL);
    halt->inputPipe = first->inputPipe;
    halt->outputPipe = first->outputPipe;
    halt->priority=0;
    halt->next=0;
    halt->prev=0;

    addProcessToList(first);
//    ncPrintChar('4');
    //newProcess(&haltP, MAX_PRIORITY); //creo proceso halt
    startFirstP();
    ncPrintChar('F');   //aca no dberia llegar
    return 0;
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
