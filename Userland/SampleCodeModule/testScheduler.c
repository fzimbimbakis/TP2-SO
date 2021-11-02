#include "lib.h"
#include "test_util.h"
//#include "testScheduler.h"

//TO BE INCLUDED
void endless_loop(){
  while(1);
}

uint32_t my_create_process(void * name){
  return newP((uint64_t)name, "endlessSch");
}

uint32_t my_kill(uint32_t pid){
  return kill(pid);
}

uint32_t my_block(uint32_t pid){
  return block(pid);
}

uint32_t my_unblock(uint32_t pid){
  return unblock(pid);
}

#define MAX_PROCESSES 100 //Should be around 80% of the the processes handled by the kernel

enum State {ERROR, RUNNING, BLOCKED, KILLED};

typedef struct P_rq{
  uint32_t pid;
  enum State state;
}p_rq;

void test_processes(){
  p_rq p_rqs[MAX_PROCESSES];
  uint8_t rq;
  uint8_t alive = 0;
  uint8_t action;

  while (1) {

      // Create MAX_PROCESSES processes
      for (rq = 0; rq < MAX_PROCESSES; rq++) {
          p_rqs[rq].pid = my_create_process(&endless_loop);

          if (p_rqs[rq].pid == -1) {
              printf("Error creating process\n");
              return;
          } else {
              p_rqs[rq].state = RUNNING;
              alive++;
          }
      }
      // Randomly kills, blocks or unblocks processes until every one has been killed
      while (alive > 0) {

          for (rq = 0; rq < MAX_PROCESSES; rq++) {
              action = GetUniform(2) % 2;

              switch (action) {
                  case 0:
                      if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == BLOCKED) {
                          if (my_kill(p_rqs[rq].pid) == -1) {
                              printf("Error killing process\n");
                              return;
                          }
                          p_rqs[rq].state = KILLED;
                          alive--;
                          //printf("Killed: %d\n", p_rqs[rq].pid);
                          //ps();
                      }
                      break;

                  case 1:
                      if (p_rqs[rq].state == RUNNING) {
                          if (my_block(p_rqs[rq].pid) == -1) {
                              printf("Error blocking process\n");
                              return;
                          }
                          p_rqs[rq].state = BLOCKED;
                      }
                      break;
              }
          }

          // Randomly unblocks processes
          for (rq = 0; rq < MAX_PROCESSES; rq++)
              if (p_rqs[rq].state == BLOCKED && GetUniform(2) % 2) {
                  if (my_unblock(p_rqs[rq].pid) == -1) {
                      printf("Error unblocking process\n");
                      return;
                  }
                  p_rqs[rq].state = RUNNING;
              }
      }
  }
}