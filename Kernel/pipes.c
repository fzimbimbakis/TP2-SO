

#include "pipes.h"


static pipe_t * firstPipe;
static int lastID = 1;

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
    char * sem = alloc(3 * sizeof(char));
    sem[0] = lastID;
    sem[1] = 'R';
    sem[2] = 0;
    if (sem_create(sem, 0) == -1) {
        free(sem);
        ncPrint("pipeOpen --> first sem_create returned -1.");
        return -1;
    }
    pipe_t* stdin = newPipe(READ, lastID++, NULL, 0, 0, sem, 0, NULL);
    pipe_t* stdout = newPipe(WRITE, lastID++, NULL, 0, 0, sem, 0, stdin);
//    pipe_t * stderr = newPipe(WRITE, lastID++, NULL, 0, 0, 0, 0, stdout);
    setReverseSide(stdout, stdin);
    setReverseSide(stdin, stdout);
//    firstPipe = stdout;
    pcb->inputPipe = stdin;
    pcb->outputPipe = stdout;
}



pipe_t * newPipe(char type, int id, char * buffer, int * nRead, int * nWrite, char * sem_R, char * sem_W, pipe_t * next){
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
        ncPrint("pipeOpen --> first sem_create returned -1.");
        return -1;
    }
//    ncPrint("despues semCreateR\n");
    char * Wid = alloc(3 * sizeof(char));
    Wid[0] = lastID;
    Wid[1] = 'W';
    Wid[2] = 0;
    if (sem_create(Wid, 0) == -1) {
        free(Wid);
        free(Rid);
        ncPrint("pipeOpen --> second sem_create returned -1.");
        return -1;
    }
//    ncPrint("despues semCreatew\n");

    char* buffer = alloc(STARTING_PIPES_SPACE * sizeof (char ));
    if(buffer==0){
        free(Wid);
        free(Rid);
        return -1;
    }

    int * nWrite = alloc(sizeof(int));
    int * nRead = alloc(sizeof(int));
    *nRead = 0;
    *nWrite = 0;

//    ncPrint("antes de newPipe\n");
    pipe_t* readPipe = newPipe(READ, lastID++, buffer, nRead, nWrite, Rid, Wid, firstPipe);
    if (readPipe == NULL){
        free(Wid);
        free(Rid);
        free(buffer);
        return -1;
    }
    pipe_t* writePipe = newPipe(WRITE, lastID++, buffer, nRead, nWrite, Rid, Wid, readPipe);
    if (writePipe == NULL){
        free(Wid);
        free(Rid);
        free(buffer);
        free(readPipe);
        return -1;
    }
//    ncPrint("despues de newPipe\n");
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
            ncPrint("Error en pipe Write. Pipe no existe o es de lectura.");
            return -1;
        }
        if(aux->buffer==NULL){      // Salida estandar posta
            for (int i = 0; i < count; ++i) {
                ncPrintChar(buffer[i]);
            }
            return 0;
        }
    }
    else {
        aux = firstPipe;
        while (aux != NULL) {
            if (aux->id == fd)
                break;
            aux = aux->next;
        }

        if (aux == NULL || aux->type == READ) {
            ncPrint("Error en pipe Write. Pipe no existe o es de lectura.");
            return -1;
        }
    }
    for (int i = 0; i < count; ++i) {
//        ncPrint("i: ");
//        ncPrintDec(i);
//        ncPrintChar(' ');
        while ((*(aux->nWrite)) == (*(aux->nRead)) + PIPE_SIZE) {      // TODO:Si otro write me gana, se me solapa lo que escribo
            if (aux->read_waiting > 0) {
                for (int j = 0; j < aux->read_waiting; ++j)
                    sem_post(aux->sem_R);
                aux->read_waiting = 0;
            }
            ncPrint("A esperar write?\n");
            aux->write_waiting++;
            sem_wait(aux->sem_W);
        }
        ncPrintDec((*(aux->nWrite)) % PIPE_SIZE);
        ncPrintChar(' ');
        aux->buffer[(*(aux->nWrite))++ % PIPE_SIZE] = buffer[i];
//        ncPrint("nWrite: ");
//        ncPrintDec((*(aux->nWrite)));
//        ncPrintChar(' ');
    }
    if (aux->read_waiting > 0) {
        ncPrint("read waiting?\n");
        for (int j = 0; j < aux->read_waiting; ++j)
            sem_post(aux->sem_R);
    }
}

int pipeClose(int fd){
    pipe_t * aux = firstPipe->next;
    pipe_t * prev = firstPipe;
    if(prev->id==fd){

        if(prev->reverse_side==NULL){
            free(prev->buffer);
            sem_close(prev->sem_W);
            sem_close(prev->sem_R);
            free(prev->nWrite);
            free(prev->nRead);
            firstPipe = prev->next;
            free(prev);
            return 0;
        } else{
            prev->reverse_side->reverse_side = NULL;
            firstPipe = prev->next;
            free(prev);
            return 0;
        }

    }
    while (aux!=NULL){
        if(aux->id==fd)
            break;
        prev = aux;
        aux = aux->next;
    }
    if(aux==NULL)
        return -1;

    if(aux->reverse_side == NULL){
        prev->next=aux->next;
        sem_close(aux->sem_W);
        sem_close(aux->sem_R);
        free(aux->nWrite);
        free(aux->nRead);
        free(aux->buffer);
        free(aux);
        return 0;
    } else{
        prev->next=aux->next;
        aux->reverse_side->reverse_side=NULL;
        free(aux);
        return 0;
    }


}

int pipeRead(int fd, char * buffer, int count){
//    ncPrint("R1\n");
    pipe_t * aux;
    if(fd==0){
        aux = getCurrentPCB()->inputPipe;
        if(aux->buffer==NULL){
            cleanBuffer();
            while (getEndBuffer() < count) {
//                ncPrint("Se bloquea\n");
//                blockProcess();
                sem_wait(aux->sem_R);
            }
            char *inBuffer = getBuffer();
            int i;
            for (i = 0; i < count; i++) {
                buffer[i] = inBuffer[i];
            }

            cleanBuffer();
            return 0;
        }
    } else {
        aux = firstPipe;
        while (aux != NULL) {
            if (aux->id == fd)
                break;
            aux = aux->next;
        }
        if (aux == NULL || aux->type == WRITE) {
            ncPrint("pipeRead: Pipe no existe o es escritura en vez de lectura.\n");
            return -1;
        }
    }
//    ncPrint("SALIO\n");
        for (int i = 0; i < count; ++i) {
//            ncPrintDec(i);
//            ncPrint(" i ");
//            ncPrintChar(' ');
//            ncPrintDec((*(aux->nWrite)));
//            ncPrint("nWrite ");
//            ncPrintChar(' ');
//            ncPrintDec((*(aux->nRead)));
//            ncPrint("nRead ");
//            ncPrint(" | ");
            while ((*(aux->nRead)) == (*(aux->nWrite))){      // TODO:Si otro write me gana, se me solapa lo que escribo
                if(aux->write_waiting>0) {
                    for (int j = 0; j < aux->write_waiting; ++j)
                        sem_post(aux->sem_W);
                    aux->write_waiting = 0;
                }
                ncPrint("A esperar read?");
                aux->read_waiting++;
                sem_wait(aux->sem_R);
            }
//            ncPrintChar(aux->buffer[(*(aux->nRead)) % PIPE_SIZE]);
//            ncPrintChar(' ');
            buffer[i] = aux->buffer[(*(aux->nRead))++ % PIPE_SIZE];
        }

        if(aux->write_waiting>0) {
            for (int j = 0; j < aux->write_waiting; ++j)
                sem_post(aux->sem_W);
        }
}

int dup(char oldId, char id){
//    ncPrint("DUP\n");
//    ncPrintDec(id);
//    ncPrintChar('\n');
    pipe_t * aux = firstPipe;
    while (aux!=NULL){
        if(aux->id==id)
            break;
        aux = aux->next;
    }
    if(aux==NULL) {
        ncPrint("Error pipe not found in dup\n");
        return -1;
    }
    if(oldId==0){
        if(aux->type!=READ) {
            ncPrint("Pipe not same type.\n");
            return -1;
        }
        getCurrentPCB()->inputPipe = aux;
        return 0;
    }
    if(oldId==1){
        if(aux->type!=WRITE) {
            ncPrint("Pipe not same type.\n");
            return -1;
        }
        getCurrentPCB()->outputPipe = aux;
        return 0;
    }
    return -1;
}


