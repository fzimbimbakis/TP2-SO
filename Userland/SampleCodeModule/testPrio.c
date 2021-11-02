#include <stdint.h>
#include "lib.h"

#define MINOR_WAIT 4000000

uint32_t _getpid(){
  return getpid();
}

uint64_t _create_process(void * name){
  return newP((uint64_t)name, "endlessPrio");
}

uint64_t _nice(uint64_t pid, uint64_t newPrio){
  return nice(pid, newPrio);
}

uint64_t _kill(uint64_t pid){
  return kill(pid);
}

uint64_t _block(uint64_t pid){
  return block(pid);
}

uint64_t _unblock(uint64_t pid){
  return unblock(pid);
}

void bussy_wait(uint64_t n){
  uint64_t i;
  for (i = 0; i < n; i++);
}

void _endless_loop(){
  uint64_t pid = _getpid();

  while(1){
    printf("%d ",pid);
    bussy_wait(MINOR_WAIT);
  }
}

#define TOTAL_PROCESSES 3

void test_prio(){
  uint64_t pids[TOTAL_PROCESSES];
  uint64_t i;

  for(i = 0; i < TOTAL_PROCESSES; i++)
    pids[i] = _create_process(&_endless_loop);

  //bussy_wait(WAIT);
  yield();
  printf("\nCHANGING PRIORITIES...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++){
    switch (i % 3){
      case 0:
        _nice(pids[i], 0); //lowest priority
        //printf("Pid %d prio 0\n",pids[i]);
        break;
      case 1:
        _nice(pids[i], 1); //medium priority
        //printf("Pid %d prio 1\n",pids[i]);
        break;
      case 2:
        _nice(pids[i], 2); //highest priority
        //printf("Pid %d prio 2\n",pids[i]);
        break;
    }
  }

  //bussy_wait(WAIT);
  yield();
  printf("\nBLOCKING...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++)
    _block(pids[i]);

  printf("CHANGING PRIORITIES WHILE BLOCKED...\n");
  for(i = 0; i < TOTAL_PROCESSES; i++){
    switch (i % 3){
      case 0:
        _nice(pids[i], 1); //medium priority 
        break;
      case 1:
        _nice(pids[i], 1); //medium priority
        break;
      case 2:
        _nice(pids[i], 1); //medium priority
        break;
    }
  }

  printf("UNBLOCKING...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++)
    _unblock(pids[i]);

  //bussy_wait(WAIT);
  yield();
  printf("\nKILLING...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++)
    _kill(pids[i]);

    printf("Test Finished\n");
}