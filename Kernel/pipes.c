

#include "pipes.h"


static pipe_t * firstPipe;
static int lastID = 0;

void setReverseSide(pipe_t * myPipe, pipe_t * reverse_side){
    myPipe->reverse_side = reverse_side;
}

void initialPipes(PCB * pcb){

    // Creo estructuras de pipes
//    char * InBuffer = alloc(sizeof(char)*PIPE_SIZE);
//    char * OutBuffer = alloc(sizeof(char)*PIPE_SIZE);
    // Semaforos de std
//    int InSem_R = sem_create(0);
//    int InSem_W = sem_create(0);
//    int OutSem_R = sem_create(0);
//    int OutSem_W = sem_create(0);

    pipe_t* stdin = newPipe(READ, lastID++, NULL, 0, 0, 0, 0, NULL);
    pipe_t* stdout = newPipe(WRITE, lastID++, NULL, 0, 0, 0, 0, stdin);
//    pipe_t * stderr = newPipe(WRITE, lastID++, NULL, 0, 0, 0, 0, stdout);
    setReverseSide(stdout, stdin);
    setReverseSide(stdin, stdout);
//    firstPipe = stdout;
    pcb->inputPipe = stdin;
    pcb->outputPipe = stdout;
}



pipe_t * newPipe(char type, int id, char * buffer, int nRead, int nWrite, char * sem_R, char * sem_W, pipe_t * next){
    pipe_t* pipe = alloc(sizeof(pipe_t));
    if (pipe == NULL)
        return NULL;
    pipe->id = id;
    pipe->type = type;
    pipe->buffer = buffer;
    pipe->nRead = nRead;
    pipe->nWrite = nWrite;
    pipe->sem_R = sem_R;
    pipe->read_waiting = 0;
    pipe->sem_W = sem_W;
    pipe->write_waiting= 0;
    pipe->next = next;
    pipe->reverse_side = NULL;
    return pipe;
}

int pipeOpen(int * array){
//    ncPrint("pipeOpen\n");
    char * Rid = alloc(3 * sizeof(char));
    Rid[0] = lastID;
    Rid[1] = 'R';
    Rid[2] = 0;
    if (sem_create(Rid, 0) == -1) {
        free(Rid);
        return -1;
    }
//    ncPrint("despues semCreateR\n");
    char * Wid = alloc(3 * sizeof(char));
    Wid[0] = lastID;
    Wid[1] = 'R';
    Wid[2] = 0;
    if (sem_create(Wid, 0) == -1) {
        free(Wid);
        free(Rid);
        return -1;
    }
//    ncPrint("despues semCreatew\n");

    char* buffer = alloc(STARTING_PIPES_SPACE * sizeof (char ));
    if(buffer==0){
        free(Wid);
        free(Rid);
        return -1;
    }

    ncPrint("antes de newPipe\n");
    pipe_t* readPipe = newPipe(READ, lastID++, buffer, 0, 0, Rid, Wid, firstPipe);
    if (readPipe == NULL){
        free(Wid);
        free(Rid);
        free(buffer);
        return -1;
    }
    pipe_t* writePipe = newPipe(WRITE, lastID++, buffer, 0, 0, Rid, Wid, readPipe);
    if (writePipe == NULL){
        free(Wid);
        free(Rid);
        free(buffer);
        free(readPipe);
        return -1;
    }
    ncPrint("despues de newPipe\n");
    firstPipe = writePipe;

    array[0]=readPipe->id;
    array[1]=writePipe->id;

    return 0;
}

int pipeWrite(int fd, char * buffer, int count){
//    ncPrint("A");
    pipe_t * aux;
//    ncPrint("B");
    if (fd==1) {
        aux = getCurrentPCB()->outputPipe;
        if(aux==NULL || aux->type==READ) {//porl
            ncPrintDec((size_t)aux);
//            ncPrintChar('\n');
            if(aux!=NULL)
            ncPrintDec(aux->type);
//            ncPrintChar('\n');
//            ncPrint("WTF");
            return -1;
        }
//        ncPrint("C");
        for (int i = 0; i < count; ++i) {
            ncPrintChar(buffer[i]);
        }
    }
    else {
        aux = firstPipe;
        while (aux != NULL) {
            if (aux->id == fd)
                break;
            aux = aux->next;
        }

        if (aux == NULL || aux->type == READ)
            return -1;

        for (int i = 0; i < count; ++i) {

            while (aux->nWrite ==aux->nRead + PIPE_SIZE) {      // TODO:Si otro write me gana, se me solapa lo que escribo
                if (aux->read_waiting > 0) {
                    for (int j = 0; j < aux->read_waiting; ++j)
                        sem_post(aux->sem_R);
                }
                aux->write_waiting++;
                sem_wait(aux->sem_W);
            }

            aux->buffer[aux->nWrite++ % PIPE_SIZE] = buffer[i];
        }
        if (aux->read_waiting > 0) {
            for (int j = 0; j < aux->read_waiting; ++j)
                sem_post(aux->sem_R);
        }
    }
}

int pipeClose(int fd){
    pipe_t * aux = firstPipe;
    pipe_t * prev;
    while (aux!=NULL){
        if(aux->id==fd)
            break;
        prev = aux;
        aux = aux->next;
    }
    if(aux==NULL || aux->type==READ)
        return -1;

    if(aux->reverse_side == NULL){
        prev->next=aux->next;
        sem_close(aux->sem_W);
        sem_close(aux->sem_R);
        free(aux->buffer);
        free(aux);
        return 0;
    } else{
        prev->next=aux->next;
        aux->reverse_side->reverse_side=NULL;
        free(aux);
    }


}

int pipeRead(int fd, char * buffer, int count){
//    ncPrint("R1\n");
    if(fd==0){
        cleanBuffer();
        while(getEndBuffer()<count){
            blockProcess();;
        }
        char * inBuffer = getBuffer();
        int i;
        for (i = 0; i < count ; i++)
        {
            buffer[i] = inBuffer[i];
        }

        cleanBuffer();
    } else{
        pipe_t * aux = firstPipe;
               while (aux!=NULL){
                   if(aux->id==fd)
                       break;
                   aux = aux->next;
               }
               if(aux==NULL || aux->type==WRITE)
                   return -1;
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

int dup(char oldId, char id){
    pipe_t * aux = firstPipe;
    while (aux!=NULL){
        if(aux->id==id)
            break;
        aux = aux->next;
    }
    if(aux==NULL)
        return -1;
    if(oldId==0){
        if(aux->type!=READ)
            return -1;
        getCurrentPCB()->inputPipe = aux;
        return 0;
    }
    if(oldId==1){
        if(aux->type!=WRITE)
            return -1;
        getCurrentPCB()->outputPipe = aux;
        return 0;
    }
    return -1;
}


