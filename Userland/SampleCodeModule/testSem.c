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
    return close_sem(sem_id);
}

#define TOTAL_PAIR_PROCESSES 2
#define SEM_ID "sem"
#define AUX_SEM "auxSem"
#define TOTAL_INCREMENT 10
static int global;  //shared memory

void slowInc(int *p, int inc){
    int aux = *p;   //auxA: 1000
    aux += inc;     //auxB 1000   998
    yield();
    *p = aux;
    yield();
    printf("#");
    return;
}

void incSemA(){
    uint64_t i;
    int value = 1;
    uint64_t N = TOTAL_INCREMENT;
//    if (!my_sem_open(SEM_ID, 1)){
//        printf("ERROR OPENING SEM\n");
//        exit();
//    }

    for (i = 0; i < N; i++){
        my_sem_wait(SEM_ID);
//        printf(" A.1 ");
        slowInc(&global, value);
//        printf(" i:%d A.2\n", i);
        my_sem_post(SEM_ID);
    }

//    my_sem_close(SEM_ID);

    printf("Final value: %d\n", global);
    my_sem_post(AUX_SEM);
    exit();
}
void incSemB(){
    uint64_t i;
    int value = -1;
    uint64_t N = TOTAL_INCREMENT;


    for (i = 0; i < N; i++){
        my_sem_wait(SEM_ID);
//        printf(" B.1 ");
        slowInc(&global, value);
//        printf(" i:%d B.2 \n", i);
        my_sem_post(SEM_ID);
    }


    printf("Final value: %d\n", global);
    my_sem_post(AUX_SEM);
    exit();
}
void incNoSemA(){
    uint64_t i;
    int value = 1;
    int N = TOTAL_INCREMENT;
    for (i = 0; i < N; i++){
        slowInc(&global, value);
    }

    printf("Final value: %d\n", global);
    exit();
}
void incNoSemB(){
    uint64_t i;
    int value = -1;
    uint64_t N = TOTAL_INCREMENT;

    for (i = 0; i < N; i++){
        slowInc(&global, value);
    }

    printf("Final value: %d\n", global);
    exit();
}

void test_sync(){
    uint64_t i;

    global = 1000;

    if ((i = my_sem_open(AUX_SEM, 0)))
        printf("ERROR OPENING SEM: %d\n", i);
    if ((i = my_sem_open(SEM_ID, 1))){
        printf("ERROR OPENING SEM: %d\n", i);
    } else{
        printf("CREATING PROCESSES...(WITH SEM)\n");

        for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
            newP(&incSemA);
            newP(&incSemB);
        }
    }
    for (int j = 0; j < TOTAL_PAIR_PROCESSES; ++j) {
        my_sem_wait(AUX_SEM);
        my_sem_wait(AUX_SEM);
    }
    my_sem_close(AUX_SEM);
    my_sem_close(SEM_ID);
    return;
}

void test_no_sync(){
    uint64_t i;

    global = 1000;

    printf("CREATING PROCESSES...(WITHOUT SEM)\n");

    for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
        newP(&incNoSemA);
        newP(&incNoSemB);
    }
    return;
}

