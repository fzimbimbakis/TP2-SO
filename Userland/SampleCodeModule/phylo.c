// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "lib.h"
#include "phylo.h"

int currentPhylos = 0;
phylo *phylos[MAX_PHYLO];
char *semForksIds[10] = {"Phylo0", "Phylo1", "Phylo2", "Phylo3", "Phylo4", "Phylo5", "Phylo6", "Phylo7", "Phylo8",
                         "Phylo9"};
char *aux[10] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
int printerPID;

void phyloCommand() {
    printf("Welcome to the phylosofers table.\n Press 'a' to add a Phylosofer.\nPress 'r' to remove a Phylosofer.\nPress 'k' to kill all phylosofers.\n");
    create_sem("PhyloMutex", 1);
    while (currentPhylos < INITIAL_PHYLO) {
        uint32_t pid = newBufferProcess((uint64_t) & phyloCycle, 1, semForksIds[currentPhylos]);
        addPhylo(pid);
    }

    while (1) {
        char c = getChar();
        if (c == 'a') {
            if (currentPhylos < MAX_PHYLO) {
                uint32_t pid = newBufferProcess((uint64_t) & phyloCycle, 0, semForksIds[currentPhylos]);
                addPhylo(pid);
            } else {
                printf("Can't add more Phylosofers\n");
            }

        } else if (c == 'r') {
            int condition = (currentPhylos == 0);
            removePhylo();
            if (condition)
                killPhylos();

        } else if (c == 'k') {
            for (int i = 0; i < currentPhylos;) {
                removePhylo();
            }
            killPhylos();
            return;
        }
    }
}

void phyloCycle(char *id) {

    id += 5;
    int idx = strToNum(id);
    while (1) {
        think();
        takeFork(phylos[idx]);
        eat();
        putFork(phylos[idx]);
    }
}

void think() {
    sleep(1);
}

void removePhylo() {
    wait_sem("PhyloMutex");
    currentPhylos--;
    phylo *p = phylos[currentPhylos];
    close_sem(p->sem);
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

}

void addPhylo(uint32_t pid) {
    wait_sem("PhyloMutex");
    char *sem = semForksIds[currentPhylos];
    create_sem(sem, 0);


    phylo *newPhylo = alloc(sizeof(phylo));
    if (newPhylo != 0) {
        newPhylo->pid = pid;
        newPhylo->state = THINKING;
        newPhylo->idx = currentPhylos;
        newPhylo->sem = sem;
        phylos[currentPhylos++] = newPhylo;


    } else printf("Error de memoria");

    post_sem("PhyloMutex");
}

void eat() {
    sleep(1);

}

void takeFork(phylo *p) {
    wait_sem("PhyloMutex");
    p->state = WAITING;
    test(p);
    post_sem("PhyloMutex");
    wait_sem(p->sem);
}

void test(phylo *p) {
    if (p->state == WAITING && phylos[LEFT]->state != EATING && phylos[RIGHT]->state != EATING) {
        p->state = EATING;
        printerProcess();
        post_sem(p->sem);
    }
}

void putFork(phylo *p) {
    wait_sem("PhyloMutex");
    p->state = THINKING;
    test(phylos[LEFT]);
    test(phylos[RIGHT]);

    post_sem("PhyloMutex");
}

void killPhylos() {
    printf("Phylo ended\n");
    close_sem("PhyloMutex");
    return;
}
