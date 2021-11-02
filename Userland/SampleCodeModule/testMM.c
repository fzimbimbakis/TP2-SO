//
// Created by bruno on 10/18/21.
//
#include <stddef.h>
#include "lib.h"
#include "testMM.h"
#include "test_util.h"

#define MAX_BLOCKS 64
#define MAX_MEMORY 500000 //Should be around 80% of memory managed by the MM

typedef struct MM_rq{
    void *address;
    uint32_t size;
}mm_rq;
void * myMemset(void * destination, int32_t c, uint64_t length)
{
    uint8_t chr = (uint8_t)c;
    char * dst = (char*)destination;

    while(length--)
        dst[length] = chr;

    return destination;
}
void test_mm(){
    mm_rq mm_rqs[MAX_BLOCKS];
    uint8_t rq;
    uint32_t total;

  while (1){
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
      sleep(1);
      printf("SET\n");
    uint32_t i;

    for (i = 0; i < rq; i++)
        if (mm_rqs[i].address != NULL)
            myMemset(mm_rqs[i].address, i, mm_rqs[i].size);
            // TODO: Chequear. No se que memset esta usando. Pero si lo defino arriba salta un error de redefinicion

    // Check
      sleep(1);
      printf("CHECK\n");
    for (i = 0; i < rq; i++)
        if (mm_rqs[i].address != NULL)
            if(!memcheck(mm_rqs[i].address, i, mm_rqs[i].size))
                printf("ERROR!\n");

    // Free
      sleep(1);
      printf("FREE\n");
    for (i = 0; i < rq; i++)
        if (mm_rqs[i].address != NULL)
            free(mm_rqs[i].address);
  }
}
