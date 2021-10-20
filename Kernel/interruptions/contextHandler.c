#include "contextHandler.h"
#include <stdint.h>
#include "../include/naiveConsole.h"

struct proces{
    char state;
    int returnValue;
}typedef proces;

static process rr[256];

int64_t handler(int64_t newRSP){
    //ncPrintChar('X'); //para chequear que este interrumpiendo bien
    proces newP;
    newP.returnValue=newRSP;
    newP.state=0;
    newP.pid=pCount;
    rr[pCount++]=newP;
    pCount=pCount%256;
    int64_t output = rr[pCurrent++].returnValue;
    pCurrent=pCurrent%256;
    return output;
}

void newProcess(int64_t sp){
    proces newP;
    newP.returnValue = sp;
    newP.state=0;
    rr[pCount++]=newP;
    pCount=pCount%256;
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
