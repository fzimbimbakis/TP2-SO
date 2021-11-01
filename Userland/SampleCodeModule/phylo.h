#ifndef TP2_SO_PHYLO_H
#define TP2_SO_PHYLO_H
#include "inttypes.h"

#define EATING 0
#define THINKING 1
#define WAITING 2
#define MAX_PHYLO 10
#define INITIAL_PHYLO 5

#define LEFT ((p->idx + currentPhylos-1) % currentPhylos)
#define RIGHT ((p->idx + 1) % currentPhylos)

typedef struct phylo{
    int pid;
    char* sem;
    int idx;
    int state;
}phylo;

void phyloCommand();
void think();
void eat();
void takeFork();
void putFork();
void addPhylo(uint32_t pid);
void printerProcess();
void removePhylo();
void phyloCycle();
void killPhylos();
void test();
#endif //TP2_SO_PHYLO_H
