#ifndef _LIB_H_
#define _LIB_H_

#include <stdint.h>

#define STDIN 0
#define STDOUT 1
#define MAX_BUFFER 40
#define STDERR 2
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
int newP(void * fPtr);
void exit();
void sleep(int seconds);
void ps();
int kill(uint32_t pid);
int block(uint32_t pid);
int unblock(uint32_t pid);
uint32_t getpid();
int nice(uint32_t pid, char newPrio);
void yield();
unsigned * memInfo();
unsigned long int hexToDec(char* num);
#include <stdint.h>
int post_sem(char * id);
int wait_sem(char * id);
int create_sem(char * id, uint64_t value);
void yield();
int close_sem(char * id);
#endif