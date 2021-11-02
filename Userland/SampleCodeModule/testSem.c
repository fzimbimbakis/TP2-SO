// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "testSem.h"


uint64_t my_sem_open(char *sem_id, uint64_t initialValue) {
    return create_sem(sem_id, initialValue);
}

uint64_t my_sem_wait(char *sem_id) {
    return wait_sem(sem_id);
}

uint64_t my_sem_post(char *sem_id) {
    return post_sem(sem_id);
}

uint64_t my_sem_close(char *sem_id) {
    return close_sem(sem_id);
}

#define TOTAL_PAIR_PROCESSES 10
#define SEM_ID "sem"
#define AUX_SEM "auxSem"
#define TOTAL_INCREMENT 25
static int global;  //shared memory
void bussyWait(uint64_t n) {
    uint64_t i;
    for (i = 0; i < n; i++);
}

void slowInc(int *p, int inc, uint32_t pid) {
    int aux = *p;   //auxA: 1000
    aux += inc;     //auxB 1000   998
    bussyWait((pid % 10) * 1000000);
    *p = aux;
    bussyWait((pid % 10) * 1000000);
    printf("#");
    return;
}

void incSemA() {
    uint64_t i;
    int value = 1;
    uint32_t pid = getpid();
    uint64_t N = TOTAL_INCREMENT;


    for (i = 0; i < N; i++) {
        my_sem_wait(SEM_ID);
        slowInc(&global, value, pid);
        my_sem_post(SEM_ID);
    }


    printf("Final value: %d\n", global);
    my_sem_post(AUX_SEM);
    exit();
}

void incSemB() {
    uint64_t i;
    int value = -1;
    uint64_t N = TOTAL_INCREMENT;
    uint32_t pid = getpid();

    for (i = 0; i < N; i++) {
        my_sem_wait(SEM_ID);
        slowInc(&global, value, pid);
        my_sem_post(SEM_ID);
    }


    printf("Final value: %d\n", global);
    my_sem_post(AUX_SEM);
    exit();
}

void incNoSemA() {
    uint64_t i;
    int value = 1;
    int N = TOTAL_INCREMENT;
    uint32_t pid = getpid();
    for (i = 0; i < N; i++) {
        slowInc(&global, value, pid);
    }

    printf("Final value: %d\n", global);
    my_sem_post(AUX_SEM);
    exit();
}

void incNoSemB() {
    uint64_t i;
    int value = -1;
    uint64_t N = TOTAL_INCREMENT;
    uint32_t pid = getpid();

    for (i = 0; i < N; i++) {
        slowInc(&global, value, pid);
    }

    printf("Final value: %d\n", global);
    my_sem_post(AUX_SEM);
    exit();
}

void test_sync() {
    uint32_t i;

    global = 1000;

    if ((i = my_sem_open(AUX_SEM, 0)))
        printf("ERROR OPENING SEM: %d\n", i);
    if ((i = my_sem_open(SEM_ID, 1))) {
        printf("ERROR OPENING SEM: %d\n", i);
    } else {
        printf("CREATING PROCESSES...(WITH SEM)\n");

        for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
            newBufferProcess((uint64_t) & incSemA,1, "testSem");
            newBufferProcess((uint64_t) & incSemB,1, "testSem");
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

void test_no_sync() {
    uint32_t i;

    global = 1000;

    printf("CREATING PROCESSES...(WITHOUT SEM)\n");
    if ((i = my_sem_open(AUX_SEM, 0)))
        printf("ERROR OPENING SEM: %d\n", i);
    for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
        newBufferProcess((uint64_t) & incNoSemA,1, "testSem nosync");
        newBufferProcess((uint64_t) & incNoSemB,1, "testSem nosync");
    }
    for (int j = 0; j < TOTAL_PAIR_PROCESSES; ++j) {
        my_sem_wait(AUX_SEM);
        my_sem_wait(AUX_SEM);
    }
    my_sem_close(AUX_SEM);
    return;
}

