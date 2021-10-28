#include "testSem.h"



uint64_t my_sem_open(char *sem_id, uint64_t initialValue){
    return create_sem(sem_id, initialValue);
}

uint64_t my_sem_wait(char *sem_id){
    return wait_sem(sem_id);
}

uint64_t my_sem_post(char *sem_id){
    return post_sem(sem_id);
}

uint64_t my_sem_close(char *sem_id){
    return 0;
}

#define TOTAL_PAIR_PROCESSES 2
#define SEM_ID "sem"

static int global;  //shared memory

void slowInc(int *p, int inc){
    int aux = *p;   //auxA: 1000
    aux += inc;     //auxB 1000   998
    yield();
    *p = aux;
    yield();
//    printf("Slow inc: %d, %d\n", aux, inc);
    return;
}

void incSemA(){
    uint64_t i;
    int value = 1;
    uint64_t N = 100;
//    if (!my_sem_open(SEM_ID, 1)){
//        printf("ERROR OPENING SEM\n");
//        exit();
//    }

    for (i = 0; i < N; i++){
        my_sem_wait(SEM_ID);
//        printf("A.1\n");
        slowInc(&global, value);
//        printf("A.2\n");
        my_sem_post(SEM_ID);
    }

    my_sem_close(SEM_ID);

    printf("Final value: %d\n", global);
    exit();
}
void incSemB(){
    uint64_t i;
    uint64_t sem = 1;
    int value = -1;
    uint64_t N = 100;
//    if (sem && !my_sem_open(SEM_ID, 1)){
//        printf("ERROR OPENING SEM\n");
//        exit();
//    }

    for (i = 0; i < N; i++){
        my_sem_wait(SEM_ID);
//        printf("B.1\n");
        slowInc(&global, value);
//        printf("B.2\n");
        my_sem_post(SEM_ID);
    }

    if (sem) my_sem_close(SEM_ID);

    printf("Final value: %d\n", global);
    exit();
}
void incNoSemA(){
    uint64_t i;
    int value = 1;
    int N = 100;
    for (i = 0; i < N; i++){
        slowInc(&global, value);
    }

    printf("Final value: %d\n", global);
    exit();
}
void incNoSemB(){
    uint64_t i;
    int value = -1;
    uint64_t N = 100;

    for (i = 0; i < N; i++){
        slowInc(&global, value);
    }

    printf("Final value: %d\n", global);
    exit();
}

void test_sync(){
    uint64_t i;

    global = 1000;
    if ((i = my_sem_open(SEM_ID, 1))){
        printf("ERROR OPENING SEM: %d\n", i);
    } else{
        printf("CREATING PROCESSES...(WITH SEM)\n");

        for(i = 0; i < 1; i++){
            newP(&incSemA, 0);
            newP(&incSemB, 0);
        }
    }
    return;
}

void test_no_sync(){
    uint64_t i;

    global = 1000;

    printf("CREATING PROCESSES...(WITHOUT SEM)\n");

    for(i = 0; i < 1; i++){
        newP(&incNoSemA, 0);
        newP(&incNoSemB, 0);
    }
    return;
}

