#include <keyboard.h>
#include <naiveConsole.h>
#include <contextHandler.h>

#define BREAK_CODE(num) num>0x80
#define IS_LETTER(c) ((c>='a') && (c<='z'))

static int idx;
static int capsLock = 0;

static const char keyTable[128] = {

      0,   27, '1', '2', '3', '4', '5', '6', '7', '8', '9',  '0', '-', '=', 
   '\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',  '[', ']', 
   '\n',    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',    
      0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',    0, '*',    
      0,  ' ',   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    0,   0,
      0,    0,   0,   0, '-',   0,   0,   0, '+',   0,   0,   0,    0,   0, 
      0,    0,   0,   0,   0,   0    

};
static char currentBuffer[64]={0};
static int bufferIdx=0;
static int endBuffer = 0;


void keyboard_handler(){
    idx = _keyHandler();
    if(endBuffer){
        currentBuffer[bufferIdx]=0;
        bufferIdx=0;
        endBuffer = 0;
    }
    if(BREAK_CODE(idx)) //es break code
        return;
    else if(idx == 0x3A)
        capsLock = 1-capsLock;
    else{
        char toPrint = keyTable[idx];
        currentBuffer[bufferIdx++]=(capsLock && IS_LETTER(toPrint))?toPrint-'a'+'A':toPrint;
        }
//        unblockShell();
//    ncPrint("Desbloqueo\n");
//    ncPrintDec(getCurrentPCB()->pid);
//    ncPrint("\n");
    sem_post(getCurrentPCB()->inputPipe->sem_R);
//    ncPrint("Paso\n");
}

void cleanBuffer(){
    endBuffer=1;
    for(int i = 0; i<64; i++){
        currentBuffer[i]=0;
    }
    (bufferIdx)=0;
}

int getEndBuffer(){
    return (bufferIdx);
}

char * getBuffer(){
    return (char*)currentBuffer;
}