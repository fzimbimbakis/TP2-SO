//
// Created by Facu Zf on 28/10/2021.
//

#include "../include/memoryManager.h"

#define MIN_ALLOC 32

#define MEM_START (unsigned)0x7FFFFF
//tamaño de 0x800000 osea 2^23

#define MEM_END (unsigned)0xFFFFFF

enum State {USED, UNUSED, SPLIT };

typedef struct node{
    struct node * left;
    struct node * right;
    unsigned size;
    unsigned logSize; //tamaño potencia de 2
    State state;
}node;



static node* root;


State recursiveAlloc(node * actual, unsigned size){

    unsigned divSize=actual->logSize/2;

    if(divSize > size)
        recursiveAlloc(actual->left, size)
}


void * alloc(unsigned size){
    if(size > root->size)
        return NULL;

    recursiveAlloc(root, size);




}


void free(void * block){

}


void init(){
    root=MEM_START;
    root->logSize=MEM_END-MEM_START;
    root->size=root->logSize-sizeof(node);
    root->state=UNUSED;
}



unsigned * memoryInfoMM(){
    unsigned * array = alloc(sizeof(unsigned ) * 3);
    unsigned totalMem = END_MEM - BEGIN_MEM;
    unsigned freeMem = 0;
    header * aux = firstHeader;
    while (aux!=NULL){
        freeMem += aux->size;
        aux = aux->ptr;
    }
    unsigned takenMem = totalMem - freeMem;
    array[0] = totalMem;
    array[1] = takenMem;
    array[2] = freeMem;
    return array;
}
