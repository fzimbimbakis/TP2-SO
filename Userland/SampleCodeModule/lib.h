#ifndef _LIB_H_
#define _LIB_H_

#define STDIN 0
#define STDOUT 1
#define MAX_BUFFER 40
#define STDERR 2
#include <stdint.h>
int strlen(char* string);
void putChar(char c);
void printf(char* string, ...);
void printDec(int num);
void printer(char* string);
int strToNum(char* str);
char* numToStr(unsigned long int num, int base);
void cleanBuffer();
void printHex(int num);
char getChar();
char* scanf();
void initial();
void* alloc(unsigned size);
void free(void * ptr);
int newP(void* fPtr, char priority);
int newBufferProcess(void* fPtr, char priority, char * buffer);
int newPipedProcess(uint64_t fPtr, char priority, char * buffer, int pipeId, char * sem_id);
void exit();
unsigned * memInfo();
unsigned long int hexToDec(char* num);
#include <stdint.h>
int post_sem(char * id);
int wait_sem(char * id);
int create_sem(char * id, uint64_t value);
void yield();
#endif