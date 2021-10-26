/* sampleCodeModule.c */
#include "lib.h"
#include "commandDispatcher.h"
#include "sysCall.h"
char * v = (char*)0xB8000 + 79 * 2;
	static char c;
	static char buffer[MAX_BUFFER]={0};
	static int idx = 0;

void imprimir(){
    while (1){
	putChar('X');
	sysWait();
	}
}

void ioi(){
	while (1){
	putChar('-');
	sysWait();
	}
}

int main() {

	printf("Bienvenido! Que comando desea correr?\n");
	//newP(&imprimir);
	//newP(&ioi);
	// while (1)
	// {
	// 	putChar('A');
	// }
	
    while(1){
		while((c=getChar())!='\n'){
			putChar(c);
			if(c=='\b'){
			if((idx)!=0)
			(idx)--;
			}
			else{
			buffer[idx]=c;
			(idx)++;
			}
		}
		putChar('\n');
		buffer[(idx)++]='\n';
		buffer[(idx)++]=0;
		idx=0;
		commandSelector(buffer);
		buffer[0] = 0; // "Borro" el buffer
	}

	return 0;
}


