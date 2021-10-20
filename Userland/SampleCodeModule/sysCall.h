#ifndef _SYSCALL_H_
#define _SYSCALL_H_

void sysWrite(int fd, char* string, int count);
void sysRead(int fd, char* string, int count);
unsigned int sysTime(unsigned int mode);
void * sysAlloc(unsigned size);
void sysFree(void * free);
unsigned * sysMemInfo();
#endif