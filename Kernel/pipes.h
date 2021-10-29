

#ifndef TP2_SO_PIPES_H
#define TP2_SO_PIPES_H
#include "semaphore.h"
#include "interruptions/keyboard.h"
#include <stddef.h>
#include "include/naiveConsole.h"
#include "interruptions/contextHandler.h"
#define STARTING_PIPES_SPACE 8
#define PIPE_SIZE 512
#define WRITE 0
#define READ !WRITE
typedef struct pipe_t{
    char type;
    char id;
    char * buffer;
    int nRead;
    int nWrite;
    char * sem_R;
    int read_waiting;
    char * sem_W;
    int write_waiting;
    struct pipe_t * reverse_side;
    struct pipe_t * next;
}pipe_t;
pipe_t * newPipe(char type, int id, char * buffer, int nRead, int nWrite, char * sem_R, char * sem_W, pipe_t * next);
void initialPipes(struct PCB * pcb);
int pipeOpen(int * array);
int pipeWrite(int fd, char* buffer, int count);
int pipeRead(int fd, char* buffer, int count);
int dup(char oldId, char id);
#endif //TP2_SO_PIPES_H
