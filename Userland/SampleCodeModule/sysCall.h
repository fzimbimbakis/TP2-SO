#ifndef _SYSCALL_H_
#define _SYSCALL_H_
#include <stdint.h>
void sysWrite(int fd, char* string, int count);
void sysRead(int fd, char* string, int count);
unsigned int sysTime(unsigned int mode);
void * sysAlloc(unsigned size);
void sysFree(void * free);
unsigned * sysMemInfo();
int sysNewP(void * fPtr, char priority);
void sysYield();
void sysBlock(char pid);
void sysWait();
int sysSemCreate(char * id, uint64_t value);
int sysSemWait(char * id);
int sysSemClose(char * id);
int sysSemPost(char * id);
void sysExit();
#endif