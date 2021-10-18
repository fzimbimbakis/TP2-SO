//
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
    printf("Set 10 blocks of size 1000\n");
    char * ptrsA[10];
    for (int i = 0; i < 10; ++i) {
        ptrsA[i] = alloc(1000);
//                printHex((int)ptrs[i]);
//                printf("\n");
        for (int j = 0; j < 1000; ++j) {
            ptrsA[i][j] = 'a'+i;
        }
    }
    printf("Check 10 blocks\n");
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
    printf("Free blocks\n");
    for (int k = 0; k < 10; ++k) {
        free(ptrsA[k]);
    }
    printf("Test passed.\n");
    return;
}
void testmm3(){
    printf("Set 10 blocks of size 1000\n");
    char * ptrs[10];
    for (int i = 0; i < 10; ++i) {
        ptrs[i] = alloc(1000);
//                printHex((int)ptrs[i]);
//                printf("\n");
        for (int j = 0; j < 1000; ++j) {
            ptrs[i][j] = 'a'+i;
        }
    }
    printf("Check 10 blocks\n");
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
    printf("Free blocks 0, 2, 4, 6 and 8\n");
    free(ptrs[0]);
    free(ptrs[2]);
    free(ptrs[4]);
    free(ptrs[6]);
    free(ptrs[8]);
    printf("Ask ans set even blocks again backwards\n");
    for (int i = 8; i >= 0; i=i-2) {
        ptrs[i] = alloc(1000);
        for (int j = 0; j < 1000; ++j) {
            ptrs[i][j] = 'a'+i;
        }
    }
    printf("Check 10 blocks\n");
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
    printf("Free blocks\n");
    for (int k = 0; k < 10; ++k) {
        free(ptrs[k]);
    }
    printf("Test passed.\n");
    return;
}