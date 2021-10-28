

#ifndef TP2_SO_PIPES_H
#define TP2_SO_PIPES_H
#include "../semaphore.h"
#include "../interruptions/keyboard.h"
#define STARTING_PIPES_SPACE 8
#define PIPE_SIZE
#define WRITE 0
#define READ !WRITE
typedef struct pipe_t{
    bool type;
    char id;
    char * buffer;
    int nRead;
    int nWrite;
    char * sem_R;
    int read_waiting;
    char * sem_W;
    int write_waiting;
    pipe_t * next
}pipe_t;
pipe_t * firstPipe;
pipe_t newPipe(bool type, int id, char * buffer, int nRead, int nWrite, int sem_R, int sem_W, pipe_t * next);
#endif //TP2_SO_PIPES_H
