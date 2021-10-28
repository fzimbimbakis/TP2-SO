

#include "pipes.h"
#include <stddef.h>
#include "../include/naiveConsole.h"
#include "../semaphore.h"
pipe_t initialPipes(){

    // Creo estructuras de pipes
//    char * InBuffer = alloc(sizeof(char)*PIPE_SIZE);
//    char * OutBuffer = alloc(sizeof(char)*PIPE_SIZE);
    // Semaforos de std
//    int InSem_R = sem_create(0);
//    int InSem_W = sem_create(0);
//    int OutSem_R = sem_create(0);
//    int OutSem_W = sem_create(0);

    pipe_t stdout = newPipe(READ,1, NULL, 0, 0, 0, 0, NULL);
    pipe_t stdin = newPipe(WRITE,0, NULL, 0, 0, 0, 0, &output);
    firstPipe = &stdin;
}

pipe_t newPipe(bool type, int id, char * buffer, int nRead, int nWrite, char * sem_R, char * sem_W, pipe_t * next){
    pipe_t pipe = alloc(sizeof(pipe_t));
    pipe.id = id;
    pipe.type = type;
    pipe.buffer = buffer;
    pipe.nRead = nRead;
    pipe.nWrite = nWrite;
    pipe.sem_R = sem_R;
    pipe.read_waiting = 0;
    pipe.sem_W = sem_W;
    pipe.write_waiting = 0;
    pipe.next = next;
    return pipe;
}

int pipeWrite(int fd, char * buffer, int count){
    pipe_t * aux = firstPipe;
    while (aux!=NULL){
        if(aux->id==fd)
            break;
        aux = aux->next;
    }
    if(aux==NULL || aux->type==READ)
        return -1;
    if(aux->buffer==NULL){
        for (int i = 0; i < count; ++i) {
            ncPrintChar(buffer[i]);
        }
    } else{
        for (int i = 0; i < count; ++i) {

            while (aux->nWrite == aux->nRead + PIPE_SIZE){      // TODO:Si otro write me gana, se me solapa lo que escribo
                if(aux->read_waiting>0) {
                    for (int j = 0; j < aux->read_waiting; ++j)
                        sem_post(aux->sem_R);
                }
                aux->write_waiting++;
                sem_wait(aux->sem_W);
            }

            aux->buffer[aux->nWrite++ % PIPE_SIZE] = buffer[i];
        }
        if(aux->read_waiting>0) {
            for (int j = 0; j < aux->read_waiting; ++j)
                sem_post(aux->sem_R);
        }
    }
}

int pipeRead(int fd, char * buffer, int count){
    pipe_t * aux = firstPipe;
    while (aux!=NULL){
        if(aux->id==fd)
            break;
        aux = aux->next;
    }
    if(aux==NULL || aux->type==WRITE)
        return -1;
    if(aux->buffer==NULL){
        cleanBuffer();
        while(getEndBuffer()<count){
            _hlt();         // TODO: Bloquear proceso en vez de _hlt?
        }
        char * inBuffer = getBuffer();
        int i;
        for (i = 0; i < count ; i++)
        {
            buffer[i] = inBuffer[i];
        }

        cleanBuffer();
    } else{
        for (int i = 0; i < count; ++i) {

            while (aux->nRead == aux->nWrite){      // TODO:Si otro write me gana, se me solapa lo que escribo
                if(aux->write_waiting>0) {
                    for (int j = 0; j < aux->write_waiting; ++j)
                        sem_post(aux->sem_W);
                }
                aux->read_waiting++;
                sem_wait(aux->sem_R);
            }

            buffer[i] = aux->buffer[aux->nRead++ % PIPE_SIZE];
        }
        if(aux->write_waiting>0) {
            for (int j = 0; j < aux->write_waiting; ++j)
                sem_post(aux->sem_W);
        }
    }
}


