#include "lib.h"
#include "phylo.h"

int currentPhylos = 0;
phylo *phylos[MAX_PHYLO];
char* semForksIds [10] = {"Phylo0","Phylo1","Phylo2","Phylo3","Phylo4","Phylo5","Phylo6","Phylo7","Phylo8","Phylo9"};
char* aux [10] = {"0","1","2","3","4","5","6","7","8","9"};
int printerPID;

void phyloCommand() {
//    create_sem("PhyloTable", 1);
    printf("Welcome to the phylosofers table.\n Press 'a' to add a Phylosofer.\nPress 'r' to remove a Phylosofer.\nPress 'k' to kill all phylosofers.\n");
    create_sem("PhyloMutex", 1);
//    printerPID = newP(&printerProcess);
    while (currentPhylos < INITIAL_PHYLO) {
//        wait_sem("PhyloTable");
        addPhylo(newBufferProcess(&phyloCycle, 0, aux[currentPhylos]));
//        post_sem("PhyloTable");
    }
    char c;
    while (1) {
        c=getChar();
        if (c == 'a') {
//            wait_sem("PhyloTable");
            addPhylo(newBufferProcess(&phyloCycle, 0, aux[currentPhylos]));
//            post_sem("PhyloTable");

        } else if (c == 'r') {
//            wait_sem("PhyloTable");
            int condition = (currentPhylos == 0);
            removePhylo();
//            post_sem("PhyloTable");
            if (condition)
                killPhylos();

        } else if (c == 'k') {
//            wait_sem("PhyloTable");
            for (int i = 0; i < currentPhylos; ) {
                removePhylo();
            }
//            post_sem("PhyloTable");
            killPhylos();
            return;
        }
    }
}

void phyloCycle(char* id){
    /*
    while (1){
        int idx=strToNum(id);
        sleep(6);
        takeFork(phylos[idx]);
        putFork(phylos[idx]);
    }*/
    int idx = strToNum(id);
//    printf("idx en cycle: %d ", idx);
    //    for (int i = 0; i < currentPhylos; ++i) {
//        if (phylos[i]->pid==pid)
//            idx = i;
//    }
    while (1){
        think();
//        printf("llego?");
        takeFork(phylos[idx]);
        eat();
        putFork(phylos[idx]);
    }
}

void think(){
//    printf("thinking \n");
    sleep(1);
//    printf(" end th\n");
}

void removePhylo() {
    wait_sem("PhyloMutex");
    currentPhylos--;
    phylo *p = phylos[currentPhylos];
    close_sem(p->sem);
//    close_sem("PhyloTable");
//    close_sem("PhyloMutex");
    kill(p->pid);
    free(p);
    post_sem("PhyloMutex");
}

void printerProcess() {
    for (int i = 0; i < currentPhylos; ++i) {
        if (phylos[i]->state == EATING)
            printf("E");
        else
            printf(".");
    }
    putChar('\n');
  /*  while (1) {
        sleep(2);

        wait_sem("PhyloTable");
        for (int i = 0; i < currentPhylos; ++i) {
            phylos[i]->state == EATING ? putChar('E') : putChar('.');
        }
        putChar('\n');
        post_sem("PhyloTable");
        sleep(2);
    }*/
}

void addPhylo(uint32_t pid) {
    wait_sem("PhyloMutex");
    char* sem = semForksIds[currentPhylos];
    create_sem(sem, 0);
//    create_sem("PhyloMutex", 1);
//    create_sem("PhyloTable", 1);


    phylo *newPhylo = alloc(sizeof(phylo));
    if (newPhylo != 0) {
        newPhylo->pid = pid;
        newPhylo->state = THINKING;
        newPhylo->idx = currentPhylos;
        newPhylo->sem = sem;
        phylos[currentPhylos++] = newPhylo;
//        printf("pid: %d", pid);
//        printf("idx: %d\n", newPhylo->idx);


    } else printf("Error de memoria");

    post_sem("PhyloMutex");
}

void eat() {
    sleep(1);
    /*
    wait_sem("PhyloTable");
    int condition = (p->state == WAITING && phylos[LEFT]->state != EATING
            && phylos[RIGHT]->state != EATING);
    post_sem("PhyloTable");
    if(condition){
        sleep(1);
        p->state = EATING;
        post_sem(p->sem);
    }*/
}

void takeFork(phylo* p) {
//    printf("phylo %d agarrando\n", p->idx);
    wait_sem("PhyloMutex");
    p->state=WAITING;
    test(p);
//    eat(p);
    post_sem("PhyloMutex");
    wait_sem(p->sem);
//    sleep(10);
}
void test(phylo * p){
    if (p->state == WAITING && phylos[LEFT]->state != EATING && phylos[RIGHT]->state != EATING){
        p->state = EATING;
        printerProcess();
        post_sem(p->sem);
    }
}

void putFork(phylo* p) {
    wait_sem("PhyloMutex");
//    sleep(10);
    p->state=THINKING;
    test(phylos[LEFT]);
    test(phylos[RIGHT]);
//    eat(phylos[LEFT]);
//    eat(phylos[RIGHT]);
    post_sem("PhyloMutex");
}

void killPhylos(){
    printf("Phylo ended\n");
//    kill(printerPID);
    close_sem("PhyloMutex");
//    close_sem("PhyloTable");
    return;
}
