#ifndef _SYSCALL_H_
#define _SYSCALL_H_
#include <stdint.h>
#include "semaphore.h"
void sysWrite(int fd, char* string, int count);
void sysRead(int fd, char* string, int count);
unsigned int sysTime(unsigned int mode);
void * sysAlloc(unsigned size);
void sysFree(void * free);
unsigned * sysMemInfo();
int sysNewP(void * fPtr, char priority);
void sysExit();
void sysYield();
void sysSleep(int seconds);
int sysBlock(uint32_t pid);
int sysUnblock(uint32_t pid);
int sysKill(uint32_t pid);
int sysNice(uint32_t pid, char newPrio);
uint32_t sysGetpid();
void sysPs();
int sysSemCreate(char * id, uint64_t value);
int sysSemWait(char * id);
int sysSemClose(char * id);
int sysSemPost(char * id);
struct sem_info_wrapper * sysSemInfo(int * qty);
#endif