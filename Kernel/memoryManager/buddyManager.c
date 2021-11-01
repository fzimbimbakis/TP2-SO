//
// Created by Facu Zf on 28/10/2021.
//

#include "../include/memoryManager.h"
#include "../include/naiveConsole.h"
#include <stdint.h>

#define MIN_ALLOC 32//2^4


#define BUDDY_START (unsigned)0xF00000
//tamaño de 2^20


#define TOTALMEM 1048576 //2^20

enum State {FULL, EMPTY, SPLIT };

typedef struct node{
    struct node * left;
    struct node * right;
    unsigned index;
    void * memPtr;
    unsigned size;
    enum State state;
}node;

#define IS_POWER_OF_2(x) (!((x)&((x)-1)))


static node* root;
unsigned memoryAllocated=0;

/* - Move to left child:     index = index * 2 + 1;
* - Move to right child:    index = index * 2 + 2;*/



static unsigned fixsize(unsigned size) {
    size |= size >> 1;
    size |= size >> 2;
    size |= size >> 4;
    size |= size >> 8;
    size |= size >> 16;
    return size+1;
}


node* createSons(node* parent){
    unsigned idx=parent->index*2+1;
    parent->left=parent + idx; //si no funciona, probar sizeof(node)

    if((uint64_t) parent->left >= BUDDY_START) {
        return NULL;
    }
    parent->left->index=idx;
    parent->left->size=parent->size/2;
    parent->left->memPtr=parent->memPtr;
    parent->left->state=EMPTY;

    uint64_t aux=(uint64_t)(parent->memPtr)+(parent->size/2); //chequear


    parent->right= parent + idx + 1;
    if((uint64_t)parent->right >= (uint64_t)BUDDY_START){
        return NULL;
    }
    parent->right->index= idx + 1;
    parent->right->size=parent->size/2;

    parent->right->memPtr=(void *)aux;
    parent->right->state=EMPTY;
}

void stateUpdate(node* actual){
    if(actual->right==NULL || actual->left==NULL){
        actual->state = EMPTY;
        return;
    }
    if(actual->left->state==FULL && actual->right->state==FULL)
        actual->state=FULL;
    else if(actual->left->state == SPLIT || actual->right->state == SPLIT ||
            actual->left->state == FULL || actual->right->state == FULL)
        actual->state=SPLIT;
    else
        actual->state=EMPTY;
}

void* allocRec(node * actual, unsigned size ){

    //ncPrintHex(actual->memPtr);

        if(actual->state==FULL) {
            //ncPrint("FULL\n");
            return NULL;
        }

        if(actual->left!=NULL || actual->right!=NULL){ //si tiene hijos es porque este nodo ya esta particionado
            //entonces debo tratar de alocar dentro de ellos
            //ncPrint("left-> ");
            void* aux= allocRec(actual->left, size);
            if(aux==NULL) {
                //ncPrint("right-> ");
                aux = allocRec(actual->right, size);
            }

            stateUpdate(actual);

            return aux;

        }
        else{//no tiene hijos y no esta lleno => está vacío

            if(size > actual->size){//no alcanza el espacio
                //ncPrint("Size is not enough\n");
                return NULL;
            }

            if(actual->size/2 >= size) {//entra en los hijos
                createSons(actual);
                //ncPrint("left-> ");
                void* aux= allocRec(actual->left, size); //no hace falta llamar al right porque son iguales y estan vacios
                stateUpdate(actual);
                return aux;

            }

            //si llegamos aca es porque el size no entra en los hijos pero si en el actual
            //ncPrintDec(actual->size);
            //ncPrint("\n");

            actual->state=FULL;
            memoryAllocated+=size;
            return actual->memPtr;
        }

}


void * alloc(unsigned size){

    //ncPrintHex(root->memPtr);

    if(size > root->size || size <= 0) {
        ncPrint("Alloc fail: Illegal size.\n");
        return NULL;
    }

    if(size < MIN_ALLOC)
        size=MIN_ALLOC;

    //ncPrint("Requested: ");
    //ncPrintDec(size);
    //ncPrint("\n");
    if(!IS_POWER_OF_2(size)){
        size=fixsize(size);
    }
//    ncPrint("Taken: ");
//    ncPrintDec(size);
//    ncPrint("\n");

    //ncPrint("Root-> ");
    void * puntero = allocRec(root, size);
    //ncPrint("puntero: ");
    //ncPrintHex(puntero);
    //ncPrint("\n");
    return puntero;
}

int freeRec(node* actual, void* block){

    int ret;

    if(actual->left!=NULL || actual->right!=NULL) {
        if ((uint64_t) actual->right->memPtr > (uint64_t) block) {
            //ncPrint("left-> ");
            ret=freeRec(actual->left, block);

        } else {
            //ncPrint("right-> ");
            ret=freeRec(actual->right, block);
        }

        stateUpdate(actual);

        if(actual->state==EMPTY){
            actual->right=NULL;
            actual->left=NULL;

        }
        return ret;


    }else if(actual->state==FULL){
        if(actual->memPtr==block) {
            //ncPrint("Libero un bloque FULL\n");
            actual->state = EMPTY;
            memoryAllocated -= actual->size;
            return 0;
        }

        return -1;

    }


    return -1;
}



void free(void * block){
    int i = freeRec(root,block);
    if(i==-1){
        ncPrint("Block to free does not exist\n");
    }
}


void init(){
    root=(node *)BEGIN_MEM;
    root->index=0;
    root->size=TOTALMEM;
    root->state=EMPTY;
    root->memPtr=(void *)BUDDY_START;
}



unsigned * memoryInfoMM(){

    unsigned * array = alloc(sizeof(unsigned ) * 3);
    unsigned totalMem = TOTALMEM;
    unsigned takenMem = memoryAllocated;
    unsigned freeMem = totalMem-takenMem;


    array[0] = totalMem;
    array[1] = takenMem;
    array[2] = freeMem;
    return array;
}
