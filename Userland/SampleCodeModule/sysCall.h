#ifndef _SYSCALL_H_
#define _SYSCALL_H_

#include <stdint.h>
#include "structs.h"

void sysWrite(int fd, char *string, int count);

void sysRead(int fd, char *string, int count);

unsigned int sysTime(unsigned int mode);

void *sysAlloc(unsigned size);

void sysFree(void *free);

unsigned *sysMemInfo();

void sysExit();

int sysNewP(uint64_t fPtr, char priority, char *arg1, int arg2, char *arg3);

void sysYield();

void sysSleep(int seconds);

int sysBlock(uint32_t pid);

int sysUnblock(uint32_t pid);

int sysKill(uint32_t pid);

int sysNice(uint32_t pid, char newPrio);

uint32_t sysGetpid();

void sysPs();

int sysPipe(int *array);

int sysDupPipe(int oldId, char id);

int sysClosePipe(int fd);

struct pipe_info_wrapper *sysInfoPipe(int *qty);

int sysSemCreate(char *id, uint64_t value);

int sysSemWait(char *id);

int sysSemClose(char *id);

int sysSemPost(char *id);

struct sem_info_wrapper *sysSemInfo(int *qty);

int sysAlterSem(char *sem_id, uint16_t value);

#endif