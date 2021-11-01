#ifndef TP2_SO_SEMAPHORE_H
#define TP2_SO_SEMAPHORE_H
typedef struct sem_info_wrapper{
    char * id;
    uint16_t value;
    char * pids;
    int nPids;
}sem_info_wrapper;
typedef struct pipe_info_wrapper{
    char id;
    sem_info_wrapper * semR;
    sem_info_wrapper * semW;
}pipe_info_wrapper;
#endif //TP2_SO_SEMAPHORE_H
