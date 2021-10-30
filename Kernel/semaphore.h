
#ifndef TP2_SO_SEMAPHORE_H
#define TP2_SO_SEMAPHORE_H
#include <stdint.h>
#include "interruptions/contextHandler.h"
#include "memoryManager.h"
#include "include/lib.h"
#include "include/spinlock.h"
#include <naiveConsole.h>
typedef struct sem_list_wrapper{
char process;
struct sem_list_wrapper * next;
}sem_list_wrapper;
typedef struct semaphore_t{
    char * id;
    uint16_t value;
    uint64_t lock;
    struct sem_list_wrapper * channel;
    int p_waiting;
    struct semaphore_t * next;
}semaphore_t;
//void sem_init();
int sem_create(char * id, uint64_t value);
int sem_post(char * sem_id);
int sem_wait(char * sem_id);
int sem_close(char * sem_id);
typedef struct sem_info_wrapper{
    char * id;
    uint16_t value;
    char * pids;
    int nPids;
}sem_info_wrapper;
struct sem_info_wrapper * sem_info(int * qty);
static int nSems = 0;
#endif //TP2_SO_SEMAPHORE_H