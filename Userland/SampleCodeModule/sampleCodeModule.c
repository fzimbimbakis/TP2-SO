/* sampleCodeModule.c */
#include "lib.h"
#include "commandDispatcher.h"
#include "sysCall.h"
char * v = (char*)0xB8000 + 79 * 2;
	static char c;
	static char buffer[MAX_BUFFER]={0};
	static int idx = 0;

//void imprimir(){
//    while (1){
//	putChar('X');
//	sysWait();
//	}
//}
//
//void ioi(){
//	while (1){
//	putChar('-');
//	sysWait();
//	}
//}

int main() {

	printf("Bienvenido! Que comando desea correr?\n");
	//newP(&imprimir);
	//newP(&ioi);
	// while (1)
	// {
	// 	putChar('A');
	// }
	char isPipedCmd = 0;
    int pipeIdx = 0;
    char isBackground = 0;
    while(1){
		while((c=getChar())!='\n'){
			putChar(c);
            if(idx==0 && c=='.'){
                isBackground = 1;
            } else{
                if (c == '\b') {
                    if ((idx) != 0)
                        (idx)--;
                } else {
                    if (c == '-') {
                        isPipedCmd = 1;
                        pipeIdx = idx;
                    }
                    buffer[idx] = c;
                    (idx)++;
                }
            }

		}
		putChar('\n');
//		buffer[(idx)++]='\n';
		buffer[(idx)++]=0;
		idx=0;
        if(!isBackground){
            if (isPipedCmd) {
                pipeCommand(buffer, pipeIdx);
            } else {
                commandSelector(buffer);
            }
        } else{
            backgroundCommand(buffer, pipeIdx, isPipedCmd);
        }
        isPipedCmd=0;
        isBackground = 0;
        pipeIdx=0;
		buffer[0] = 0; // "Borro" el buffer
	}

	return 0;
}


