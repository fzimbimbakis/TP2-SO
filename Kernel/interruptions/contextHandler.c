#include "contextHandler.h"
#include <stdint.h>
#include "../include/naiveConsole.h"

struct process{
    char state;
    uint64_t* rbp;
    uint64_t* rsp;
    int pid
}typedef process;

static process rr[256];
static int pidCount=0, pCount=0, pCurrent=0;

int64_t* handler(int64_t* newRSP){
    //ncPrintChar('X'); //para chequear que este interrumpiendo bien
    process newP;
    newP.rsp=newRSP;
    newP.state=0;
    newP.pid=pidCount;
    rr[pCount++]=newP;
    pCount=pCount%256;
    int64_t* output = rr[pCurrent++].rsp;
    pCurrent=pCurrent%256;
    return output;
}

uint64_t* newProcess(void* fPtr){
    // ncPrintChar('A');
    uint64_t* bp = alloc(1000 * sizeof(uint64_t));
    // ncPrintChar('B');
    uint64_t * sp = _createStack((uint64_t)&sp[999], fPtr);
    // ncPrintChar('C');
    process newP;
    newP.rsp = sp;
    newP.rbp = bp;
    newP.state=0;
    rr[pCount++]=newP;
    pCount=pCount%256;
    return newP.rsp;
}

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
