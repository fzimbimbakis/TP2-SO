
#ifndef TP2_SO_SEMAPHORE_H
#include <stdint.h>
#include "interruptions/contextHandler.h"
#include "memoryManager.h"
#include "include/lib.h"
#include <naiveConsole.h>
typedef struct sem_list_wrapper{
char process;
struct sem_list_wrapper * next;
}sem_list_wrapper;
typedef struct semaphore_t{
    char * id;
    uint16_t value;
//    char lock;
    struct sem_list_wrapper * channel;
    int p_waiting;
    struct semaphore_t * next;
}semaphore_t;
//void sem_init();
int sem_create(char * id, uint64_t value);
int sem_post(char * sem_id);
int sem_wait(char * sem_id);
#define TP2_SO_SEMAPHORE_H

#endif //TP2_SO_SEMAPHORE_H
