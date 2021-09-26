#include <keyboard.h>
#include <naiveConsole.h>

#define BREAK_CODE(num) num>0x80
#define IS_LETTER(c) ((c>='a') && (c<='z'))

static int idx;
static int capsLock = 0;

static const char keyTable[] = {
	0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '\'', '¿',0, 0,
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '´', '+',0, 0, 
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'ñ', '{', '|',0, '}',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '-', 
    0, '*', 0, ' ',0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '7','↑', '9', '-', '←', '5', '→', '+', '1', '↓', '3', 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0
};
static char* currentBuffer[64]={0};
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
    else if(idx == 0x1C) {   //code del enter
        currentBuffer[(bufferIdx)++]='\n';
    }
    else if(idx == 0x3A){
        capsLock = 1-capsLock;
    }
    else if(idx == 0x0E){   //code del delete
        currentBuffer[(bufferIdx)++]='\b';
    }
    else if(idx == 0xF){    //code del TAB
        currentBuffer[(bufferIdx)++]='\t';
    }
    else{
        char toPrint = keyTable[idx];
        currentBuffer[(bufferIdx)++]=(capsLock && IS_LETTER(toPrint))?toPrint-'a'+'A':toPrint;
        }
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
    return currentBuffer;
}