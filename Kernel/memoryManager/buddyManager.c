// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com


#include "../include/memoryManager.h"
#include "../include/naiveConsole.h"
#include <stdint.h>

#define MIN_ALLOC 32//2^4


#define BUDDY_START (unsigned)0xF00000
//tamaño de 2^20


#define TOTALMEM 1048576 //2^20

enum State {
    FULL, EMPTY, SPLIT
};

typedef struct node {
    struct node *left;
    struct node *right;
    unsigned index;
    void *memPtr;
    unsigned size;
    enum State state;
} node;

#define IS_POWER_OF_2(x) (!((x)&((x)-1)))


static node *root;
unsigned memoryAllocated = 0;

/* - Move to left child:     index = index * 2 + 1;
* - Move to right child:    index = index * 2 + 2;*/



static unsigned fixsize(unsigned size) {
    size |= size >> 1;
    size |= size >> 2;
    size |= size >> 4;
    size |= size >> 8;
    size |= size >> 16;
    return size + 1;
}


void createSons(node *parent) {
    unsigned idx = parent->index * 2 + 1;
    parent->left = parent + idx;

    if ((uint64_t) parent->left >= BUDDY_START) {
        return NULL;
    }
    parent->left->index = idx;
    parent->left->size = parent->size / 2;
    parent->left->memPtr = parent->memPtr;
    parent->left->state = EMPTY;

    uint64_t aux = (uint64_t)(parent->memPtr) + (parent->size / 2);


    parent->right = parent + idx + 1;
    if ((uint64_t) parent->right >= (uint64_t)BUDDY_START){
        return NULL;
    }
    parent->right->index = idx + 1;
    parent->right->size = parent->size / 2;

    parent->right->memPtr = (void *) aux;
    parent->right->state = EMPTY;
}

void stateUpdate(node *actual) {
    if (actual->right == NULL || actual->left == NULL) {
        actual->state = EMPTY;
        return;
    }
    if (actual->left->state == FULL && actual->right->state == FULL)
        actual->state = FULL;
    else if (actual->left->state == SPLIT || actual->right->state == SPLIT ||
             actual->left->state == FULL || actual->right->state == FULL)
        actual->state = SPLIT;
    else
        actual->state = EMPTY;
}

void *allocRec(node *actual, unsigned size) {


    if (actual->state == FULL) {
        //ncPrint("FULL\n");
        return NULL;
    }

    if (actual->left != NULL || actual->right != NULL) { //si tiene hijos es porque este nodo ya esta particionado
        //entonces debo tratar de alocar dentro de ellos
        void *aux = allocRec(actual->left, size);
        if (aux == NULL) {
            aux = allocRec(actual->right, size);
        }

        stateUpdate(actual);

        return aux;

    } else {//no tiene hijos y no esta lleno => está vacío

        if (size > actual->size) {//no alcanza el espacio
            return NULL;
        }

        if (actual->size / 2 >= size) {//entra en los hijos
            createSons(actual);
            void *aux = allocRec(actual->left, size); //no hace falta llamar al right porque son iguales y estan vacios
            stateUpdate(actual);
            return aux;

        }
        //si llegamos aca es porque el size no entra en los hijos pero si en el actual
        actual->state = FULL;
        memoryAllocated += size;
        return actual->memPtr;
    }

}


void *alloc(unsigned size) {

    if (size > root->size) {
        ncPrint("Alloc fail: Illegal size.\n");
        return NULL;
    }

    if (size < MIN_ALLOC)
        size = MIN_ALLOC;

    if (!IS_POWER_OF_2(size)) {
        size = fixsize(size);
    }

    void *puntero = allocRec(root, size);
    return puntero;
}

int freeRec(node *actual, void *block) {


    if (actual->left != NULL || actual->right != NULL) {
        int ret;
        if (actual->right!=0 && (uint64_t) actual->right->memPtr > (uint64_t) block) {
            ret = freeRec(actual->left, block);

        } else {
            ret = freeRec(actual->right, block);
        }

        stateUpdate(actual);

        if (actual->state == EMPTY) {
            actual->right = NULL;
            actual->left = NULL;

        }
        return ret;


    } else if (actual->state == FULL) {
        if (actual->memPtr == block) {
            actual->state = EMPTY;
            memoryAllocated -= actual->size;
            return 0;
        }

        return -1;

    }


    return -1;
}


void free(void *block) {
    int i = freeRec(root, block);
    if (i == -1) {
        ncPrint("Block to free does not exist\n");
    }
}


void init() {
    root = (node *) BEGIN_MEM;
    root->index = 0;
    root->size = TOTALMEM;
    root->state = EMPTY;
    root->memPtr = (void *) BUDDY_START;
}


unsigned *memoryInfoMM() {

    unsigned *array = alloc(sizeof(unsigned) * 3);
    if (array==0) return 0;
    unsigned totalMem = TOTALMEM;
    unsigned takenMem = memoryAllocated;
    unsigned freeMem = totalMem - takenMem;


    array[0] = totalMem;
    array[1] = takenMem;
    array[2] = freeMem;
    return array;
}
