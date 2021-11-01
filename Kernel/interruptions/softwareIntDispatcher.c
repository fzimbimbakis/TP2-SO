#include "../include/naiveConsole.h"
#include <interrupts.h>
#include <keyboard.h>
#include "../memoryManager.h"
#include "contextHandler.h"
#include "../semaphore.h"
#include "process.h"
#include "time.h"
#include "../pipes.h"

#define RED 4
typedef int (*EntryPoint)(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);

void write(unsigned int fd, const char * buffer, unsigned int count);
void read(unsigned int fd, char * buffer, unsigned int count);
int accessClock(unsigned int mode);
void memoryFree(void * ptr);
void * memoryAlloc(unsigned size);
unsigned * memoryInfo();

//EntryPoint functionPtrs[] = {&write, &read, &accessClock, &memoryAlloc, &memoryFree, &memoryInfo, &newProcess, &exit, &_hlt, &blockProcess};
EntryPoint functionPtrs[] = {(EntryPoint)&write, (EntryPoint)&read, (EntryPoint)&accessClock, (EntryPoint)&memoryAlloc, (EntryPoint)&memoryFree, (EntryPoint)&memoryInfo,
                             (EntryPoint)&newProcess, (EntryPoint)&exit, (EntryPoint)&blockProcessPID, (EntryPoint)&yield, (EntryPoint)&kill, (EntryPoint)&printProcesses,
                             (EntryPoint)&getPid, (EntryPoint)&changePriority, (EntryPoint)&sem_create, (EntryPoint)&sem_wait, (EntryPoint)&sem_post, (EntryPoint)&sem_close,(EntryPoint)&sem_info, (EntryPoint)&dup,(EntryPoint)&pipeOpen, (EntryPoint)&unblockProcessPID, (EntryPoint)&sleep,
                             (EntryPoint)&pipeClose, (EntryPoint)&infoPipe, (EntryPoint)&alterSem};
//EntryPoint functionPtrs[] = {&write, &read, &accessClock, &memoryAlloc, &memoryFree, &memoryInfo,
//                             &newProcess, &exit, &blockProcessPID, &yield, &kill, &printProcesses,
//                             &getPid, &changePriority, &sem_create, &sem_wait, &sem_post, &sem_close,&sem_info, NULL,NULL, &unblockProcessPID, &sleep };

int int_80(unsigned int arg1, unsigned int arg2, unsigned int arg3, unsigned int arg4, unsigned int arg5, int sysCall){
    int ret=functionPtrs[sysCall](arg1, arg2, arg3, arg4, arg5);
    return ret;
}

void * memoryAlloc(unsigned size){
    return alloc(size);
}

void memoryFree(void * ptr){
    free(ptr);
}

unsigned * memoryInfo(){
    unsigned * array = memoryInfoMM();
    return array;
}

void write(unsigned int fd, const char * buffer, unsigned int count){
//    if(fd==1){      // STDOUT
//        for (int i = 0; i < count && buffer[i]; i++)
//        {
//            ncPrintChar(buffer[i]);
//        }
//    }
//    if(fd==2){      // STDERR
//        for (int i = 0; i < count && buffer[i]; i++)
//        {
//            ncPrintColorChar(buffer[i], RED);
//        }
//    }
    pipeWrite((int)fd, buffer, (int)count);
}

void read(unsigned int fd, char * buffer, unsigned int count){
//    if(fd==0){      // STDIN
//        cleanBuffer();
//        while(getEndBuffer()<count){
//		    blockProcess();
//        }
//        char * inBuffer = getBuffer();
//        int i;
//        for (i = 0; i < count ; i++)
//        {
//            buffer[i] = inBuffer[i];
//        }
//        cleanBuffer();
//    }
    pipeRead((int)fd, buffer, (int)count);
}