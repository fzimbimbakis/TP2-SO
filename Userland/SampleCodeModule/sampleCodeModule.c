/* sampleCodeModule.c */
#include "lib.h"
#include "commandDispatcher.h"
char * v = (char*)0xB8000 + 79 * 2;
	static char c;
	static char buffer[MAX_BUFFER]={0};
	static int idx = 0;
	
	

int main() {
	
	char* ptr = (char*) alloc(1000*sizeof(char));
	printf("ptr: %d\n", ptr);
	for (int i = 0; i < 15; i++)
	{
		ptr[i]='a';
	}
	putChar('\n');
	for (int i = 0; i < 10; i++)
	{
		putChar(ptr[i]);
		// putChar('x');
	}
	ptr = (char*) alloc(20*sizeof(char));
	printf("ptr: %d\n", ptr);
	for (int i = 0; i < 15; i++)
	{
		ptr[i]='a';
	}
	putChar('\n');
	
	
	printf("Bienvenido! Que comando desea correr?\n");
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


