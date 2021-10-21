//int n
// Created by bruno on 10/18/21.
//
#include <stddef.h>
#include "lib.h"
#include "testMM.h"
void testmm0(){
    int nroA = END_MEM - (BEGIN_MEM+100);
    char * ptrA = (char *)alloc(nroA);
    if(alloc(nroA) != NULL){
        printf("ERROR: alloc sin espacio no dio null.\n");
        free(ptrA);
        return;
    }
    for (int i = 0; i < nroA; ++i) {
        ptrA[i] = 'A';
    }
    ptrA = (char *)BEGIN_MEM+100;
    for (int i = 0; i < nroA; ++i) {
        if(ptrA[i]!='A'){
            printf("ERROR: Set all memory and check it.\n");
            return;
        }
    }
    printf("Test passed.\n");
    free(ptrA);
}
void testmm1(){
    int nro = END_MEM - (BEGIN_MEM+100);
    char * ptr = (char *)alloc(nro);
    if(alloc(nro) != NULL){
        printf("ERROR: alloc sin espacio no dio null.\n");
        free(ptr);
        return;
    }
    free(ptr);
    ptr = (char *)alloc(nro);
    if(ptr==NULL){
        printf("ERROR: alloc con espacio dio null.\n");
    }
    printf("Test passed.\n");
    free(ptr);
}
void testmm2(){
    char * ptrsA[10];
    for (int i = 0; i < 10; ++i) {
        ptrsA[i] = alloc(1000);
//                printHex((int)ptrs[i]);
//                printf("\n");
        for (int j = 0; j < 1000; ++j) {
            ptrsA[i][j] = 'a'+i;
        }
    }
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 1000; ++j) {
            if(ptrsA[i][j]!=('a'+i)){
                printf("ERROR: Blocks check. Block nro: %d", i);
                for (int k = 0; k < 10; ++k) {
                    free(ptrsA[k]);
                }
                return;
            }
        }
    }

    for (int k = 0; k < 10; ++k) {
        free(ptrsA[k]);
    }
    printf("Test passed.\n");
    return;
}
void testmm3(){

    char * ptrs[10];
    for (int i = 0; i < 10; ++i) {
        ptrs[i] = alloc(1000);
//                printHex((int)ptrs[i]);
//                printf("\n");
        for (int j = 0; j < 1000; ++j) {
            ptrs[i][j] = 'a'+i;
        }
    }

    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 1000; ++j) {
            if(ptrs[i][j]!=('a'+i)){
                printf("ERROR: Blocks check. Block nro: %d", i);
                for (int k = 0; k < 10; ++k) {
                    free(ptrs[k]);
                }
                return;
            }
        }
    }

    free(ptrs[0]);
    free(ptrs[2]);
    free(ptrs[4]);
    free(ptrs[6]);
    free(ptrs[8]);

    for (int i = 8; i >= 0; i=i-2) {
        ptrs[i] = alloc(1000);
        for (int j = 0; j < 1000; ++j) {
            ptrs[i][j] = 'a'+i;
        }
    }

    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 1000; ++j) {
            if(ptrs[i][j]!=('a'+i)){
                printf("ERROR: Blocks check. Block nro: %d", i);
                for (int k = 0; k < 10; ++k) {
                    free(ptrs[k]);
                }
                return;
            }
        }
    }

    for (int k = 0; k < 10; ++k) {
        free(ptrs[k]);
    }
    printf("Test passed.\n");
    return;
}

#include "test_util.h"

#define MAX_BLOCKS 128
#define MAX_MEMORY 8388560 //Should be around 80% of memory managed by the MM

typedef struct MM_rq{
    void *address;
    uint32_t size;
}mm_rq;

void test_mm(){
    mm_rq mm_rqs[MAX_BLOCKS];
    uint8_t rq;
    uint32_t total;

//  while (1){
    rq = 0;
    total = 0;

    // Request as many blocks as we can
    while(rq < MAX_BLOCKS && total < MAX_MEMORY){
        mm_rqs[rq].size = GetUniform(MAX_MEMORY - total - 1) + 1;
        mm_rqs[rq].address = alloc(mm_rqs[rq].size);

        if(mm_rqs[rq].address == NULL){
            printf("ERROR!\n");
        }
        total += mm_rqs[rq].size;
        rq++;
    }

    // Set
    uint32_t i;
    for (i = 0; i < rq; i++)
        if (mm_rqs[i].address != NULL)
            memset(mm_rqs[i].address, i, mm_rqs[i].size);
            // TODO: Chequear. No se que memset esta usando. Pero si lo defino arriba salta un error de redefinicion

    // Check
    for (i = 0; i < rq; i++)
        if (mm_rqs[i].address != NULL)
            if(!memcheck(mm_rqs[i].address, i, mm_rqs[i].size))
                printf("ERROR!\n");

    // Free
    for (i = 0; i < rq; i++)
        if (mm_rqs[i].address != NULL)
            free(mm_rqs[i].address);
//  }
    printf("Test passed.\n");
}
