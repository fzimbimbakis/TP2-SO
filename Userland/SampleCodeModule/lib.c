// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "lib.h"
#include "sysCall.h"
#include <stdarg.h>


#define BEGIN_MEM 0x60000
#define END_MEM 0xFFFFFFFFFFFFFFFF

char buffer[MAX_BUFFER] = {0};


int strlen(char *string) {
    int i = 0;
    while (*string) {
        i++;
        string++;
    }
    return i;
}

void cleanBuffer() {
    for (int i = 0; i < MAX_BUFFER; i++) {
        buffer[i] = 0;
    }
}

unsigned long int hexToDec(char *string) {
    unsigned long int resp = 0;
    while (*string) {
        resp *= 16;
        if (!((*string >= '0' && *string <= '9') || (*string >= 'a' && *string <= 'f')))
            return -1;
        resp += (*string >= '0' && *string <= '9') ? *string - '0' : *string - 'a' + 10;
        string++;
    }

    return resp;
}

static char hexArray[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

char *numToStr(unsigned long int num, int base) {
    cleanBuffer();
    int aux = num % base;
    int i = MAX_BUFFER - 1;
    buffer[i--] = 0;
    if (num == 0)
        buffer[i--] = '0';
    while (i >= 0 && num > 0) {
        aux = num % base;
        buffer[i] = hexArray[aux];
        i--;
        num /= base;
    }

    return buffer + i + 1;
}

int strToNum(char *str) {
    int num = 0;

    for (int i = 0; str[i]; i++) {
        num *= 10;
        if (str[i] < '0' || str[i] > '9')//no es un numero valido
            return -1;
        num += str[i] - '0';
    }
    return num;
}

void printf(char *string, ...) {
    int i = 0, argumentCount = 0;
    while (string[i]) {
        if (string[i] == '%') argumentCount++;
        i++;
    }
    va_list list;
    va_start(list, string);
    while (*string) {
        if (*string == '\'') {
            string++;
            sysWrite(STDOUT, string, 1);
            string++;
        } else if (*string != '%') {
            sysWrite(STDOUT, string, 1);
            string++;
        } else {
            string++;
            switch (*string) {
                case 'd':
                    printDec(va_arg(list,
                    int));
                    break;
                case 'c':
                    putChar(va_arg(list,
                    int));
                    break;
                case 's':
                    printf(va_arg(list,
                    char *));
                    break;
                default:
                    break;
            }
            string++;
        }
    }
    va_end(list);
}

void printDec(int num) {
    printf(numToStr(num, 10));
}

void printHex(int num) {
    printf(numToStr(num, 16));
}


void putChar(char c) {
    sysWrite(STDOUT, &c, 1);
}

char getChar() {
    char ret;
    sysRead(STDIN, &ret, 1);
    return ret;
}

int newPipedProcess(uint64_t fPtr, char isBackground, char *buffer, int pipeId, char *sem_id) {
    return sysNewP(fPtr, isBackground, buffer, pipeId, sem_id);  // No devuelve nada.
}

int newBufferProcess(uint64_t fPtr, char isBackground, char *buffer) {
    return sysNewP(fPtr, isBackground, buffer, -1, 0);  // No devuelve nada.
}

int newP(uint64_t fPtr, char *name) {
    return sysNewP(fPtr, 0, name, -1, 0);  // No devuelve nada.
}


/* no la usamos pero cppcheck nos tira el warning de style
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
}*/

void *alloc(unsigned size) {
    return sysAlloc(size);
}

void exit() {
    sysExit();
}

void sleep(int seconds) {
    sysSleep(seconds);
}

int kill(uint32_t pid) {
    return sysKill(pid);
}

int block(uint32_t pid) {
    return sysBlock(pid);
}

int unblock(uint32_t pid) {
    return sysUnblock(pid);
}

void yield() {
    sysYield();
}

void free(void *ptr) {
    sysFree(ptr);
}

unsigned *memInfo() {
    return sysMemInfo();
}

void ps() {
    sysPs();
}

uint32_t getpid() {
    return sysGetpid();
}

int nice(uint32_t pid, char newPrio) {
    return sysNice(pid, newPrio);
}

int create_sem(char *id, uint64_t value) {
    return sysSemCreate(id, value);
}

int wait_sem(char *id) {
    return sysSemWait(id);
}

int post_sem(char *id) {
    return sysSemPost(id);
}

int close_sem(char *id) {
    return sysSemClose(id);
}
