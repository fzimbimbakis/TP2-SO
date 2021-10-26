
#ifndef TP2_SO_SEMAPHORE_H
#include <stdint.h>
#include "../interruptions/contextHandler.h"
#include "../memoryManager.h"
#include "../include/lib.h"
typedef struct sem_list_wrapper{
char process;
sem_list_wrapper * next;
}sem_list_wrapper;
typedef struct sem_t {
    char * id;
    uint16_t value = 0;
    char lock = 0;
    sem_list_wrapper * channel;
    int p_waiting = 0;
    sem_t * next;
}sem_t;
void sem_init();
int sem_create(uint64_t value);
int sem_post(int sem_id);
int sem_wait(int sem_id);
#define TP2_SO_SEMAPHORE_H

#endif //TP2_SO_SEMAPHORE_H
