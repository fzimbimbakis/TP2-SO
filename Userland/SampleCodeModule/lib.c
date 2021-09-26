#include "lib.h"
#include "sysCall.h"
#include <stdarg.h>
char buffer[MAX_BUFFER] = {0};


int strlen(char* string){
	int i =0;
	while(*string){
		i++;
		string++;
	}
	return i;
}

void cleanBuffer(){
	for(int i=0; i < MAX_BUFFER; i++){
		buffer[i]=0;
	}
}

unsigned long int hexToDec(char* string){
	unsigned long int resp=0;
	while(*string){
		resp*=16;
		if(! ((*string>='0' && *string<='9') || (*string>='a' && *string<='f')) )
			return -1;
		resp+=(*string>='0' && *string<='9')?*string-'0':*string-'a'+10;
		string++;
	}

	return resp;
}

static char hexArray[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
char* numToStr(unsigned long int num, int base){
	cleanBuffer();
    char aux = num%base;
    int i = MAX_BUFFER;
    buffer[i]=0;
	if(num==0)
		buffer[i--]='0';
    while(i>=0 && num>0){
        aux = num%base;
        buffer[i]=hexArray[aux];
        i--;
        num/=base;
    }

    return buffer+i+1;
}

int strToNum(char* str){
    int num=0;
    
    for(int i=0; str[i] ; i++){
        num*=10;
        if(str[i]<'0' || str[i]>'9' )//no es un numero valido
            return -1;
        num+=str[i]-'0';
    }
    return num;
}

void printf(char* string, ...){
	int i=0, argumentCount=0;
	while(string[i]){
		if(string[i] == '%') argumentCount++;
		i++;
	}
	va_list list;
	va_start(list, argumentCount);
	while(*string){
		if(*string=='\''){
			string++;
			sysWrite(STDOUT,string, 1);
			string++;
		}
		else if(*string!='%'){
			sysWrite(STDOUT,string, 1);
			string++;
		}
		else{
			string++;
			switch (*string){
			case 'd': printDec(va_arg(list, int));
				break;
			case 'c': putChar(va_arg(list, int));
				break;
			case 's': printf(va_arg(list, int));
				break;
			default:
				break;
			}
			string++;
		}
	}
	va_end(list);
}

void printDec(int num){
	printf(numToStr(num,10));
}

void printHex(int num){
	printf(numToStr(num,16));
}

void printer(char* string){
	sysWrite(STDERR, string, strlen(string));
}

void putChar(char c){
	sysWrite( STDOUT, &c, 1);
}

char getChar(){
	char ret;
	sysRead(STDIN, &ret, 1);
	return ret;
}

char* scanf(){
	buffer[0]=0;
	static int idx = 0;
	static char c;
	while((c=getChar())!='\n'){
			if(c=='\t')
				continue;
			putChar(c);
			if(c=='\b')
			idx--;
			else{
			buffer[idx]=c;
			(idx)++;
			}
		}
		putChar('\n');
		buffer[(idx)++]='\n';
		buffer[(idx)++]=0;
	return buffer;
}
