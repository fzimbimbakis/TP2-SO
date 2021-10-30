#include "lib.h"
#include <stddef.h>
#include "sysCall.h"
#include <stdarg.h>


#define BEGIN_MEM 0x60000
#define END_MEM 0xFFFFFFFFFFFFFFFF

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

int newP(void* fPtr){
	return sysNewP(fPtr ,0);
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

void * alloc(unsigned size){
	return sysAlloc(size);
}

void exit(){
	sysExit();
}

void sleep(int seconds){
    sysSleep(seconds);
}

int kill(uint32_t pid){
	return sysKill(pid);
}

int block(uint32_t pid){
	return sysBlock(pid);
}

int unblock(uint32_t pid){
    return sysUnblock(pid);
}

void yield(){
	sysYield();
}

void free(void * ptr){
	sysFree(ptr);
}

unsigned * memInfo(){
    return sysMemInfo();
}

void ps(){
	sysPs();
}

uint32_t getpid(){
	return sysGetpid();
}

int nice(uint32_t pid, char newPrio){
	return sysNice(pid, newPrio);
}

/*

typedef long Align;
// For alignment for long values ???????

union header {
    struct
    {
        union header * ptr;
        unsigned size;
    } s;
    Align x;
} ;
typedef union header Header;

static Header * freeList = 0x600000;

void initial(){
    Header base;
    base.s.ptr = 0;
    base.s.size = ((unsigned long int)0xFFFFFFF - (unsigned long int) 6291456);
    *freeList = base;
}

void * alloc( unsigned nbytes){
    Header * Hpointer, * prevHpointer;
    unsigned nunits;

    nunits = (nbytes+sizeof(Header)-1)+ 1;
    Hpointer = freeList;
    prevHpointer = freeList;

	if(freeList == NULL)
	{
		return 0;
	}
	
    do
    {
        if(Hpointer->s.size >= nunits){
            if(Hpointer->s.size == nunits){
                if(prevHpointer != freeList)
                prevHpointer->s.ptr = Hpointer->s.ptr;
                else freeList = Hpointer->s.ptr;
            } else {
                Hpointer->s.size -= nunits;
                Hpointer += Hpointer->s.size;            // No entiendo estas dos lineas
                Hpointer->s.size = nunits;
            }
            return (void *) (Hpointer+1);
        }
        if(Hpointer->s.ptr==0)
            return 0;
        prevHpointer = Hpointer;
        Hpointer = Hpointer->s.ptr;
    } while (1);

}*/