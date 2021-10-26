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
void sysExit();
void sysYield();
int sysBlock(uint32_t pid);
int sysKill(uint32_t pid);
int sysNice(uint32_t pid, char newPrio);
uint32_t sysGetpid();
void sysPs();

void sysWait();//hlt para testear habria que borrarlo al final
#endif