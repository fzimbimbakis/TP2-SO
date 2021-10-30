#ifndef TP2_SO_SEMAPHORE_H
#define TP2_SO_SEMAPHORE_H
typedef struct sem_info_wrapper{
    char * id;
    uint16_t value;
    char * pids;
    int nPids;
}sem_info_wrapper;
#endif //TP2_SO_SEMAPHORE_H
