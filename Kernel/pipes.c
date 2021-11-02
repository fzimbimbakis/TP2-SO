// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com



#include "pipes.h"


static pipe_t *firstPipe;
static int lastID = 0;
static pipe_t *inputP;
static pipe_t *outputP;

void setReverseSide(pipe_t *myPipe, pipe_t *reverse_side) {
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
    sem[0] = lastID+1;
    sem[1] = 'R';
    sem[2] = 0;
    if (sem_create(sem, 0) == -1) {
        free(sem);
        ncPrint("pipeOpen --> first sem_create returned -1.");
        return;
    }
    pipe_t* stdin = newPipe(READ, lastID++, NULL, 0, 0, sem, 0, NULL, NULL, NULL);
    pipe_t* stdout = newPipe(WRITE, lastID++, NULL, 0, 0, sem, 0, stdin, NULL, NULL);
//    pipe_t * stderr = newPipe(WRITE, lastID++, NULL, 0, 0, 0, 0, stdout);
    setReverseSide(stdout, stdin);
    setReverseSide(stdin, stdout);
//    firstPipe = stdout;
    inputP = stdin;
    outputP = stdout;
    pcb->inputPipe = stdin;
    pcb->outputPipe = stdout;
}


pipe_t *newPipe(char type, int id, char *buffer, int *nRead, int *nWrite, char *sem_R, char *sem_W, pipe_t *next,
                int *read_waiting, int *write_waiting) {
    pipe_t *pipe = alloc(sizeof(pipe_t));
    if (pipe == NULL)
        return NULL;
    pipe->id = id;
    pipe->type = type;
    pipe->buffer = buffer;
    pipe->nRead = nRead;
    pipe->nWrite = nWrite;
    pipe->sem_R = sem_R;
    pipe->read_waiting = read_waiting;
    pipe->sem_W = sem_W;
    pipe->write_waiting = write_waiting;
    pipe->next = next;
    pipe->reverse_side = NULL;
    return pipe;
}

int pipeOpen(int * array){
//    ncPrint("pipeOpen\n");
    char * Rid = alloc(3 * sizeof(char));
    Rid[0] = lastID+1;
    Rid[1] = 'R';
    Rid[2] = 0;
    if (sem_create(Rid, 0) == -1) {
        free(Rid);
        ncPrint("pipeOpen --> first sem_create returned -1.");
        return -1;
    }
//    ncPrint("despues semCreateR\n");
    char * Wid = alloc(3 * sizeof(char));
    Wid[0] = lastID+1;
    Wid[1] = 'W';
    Wid[2] = 0;
    if (sem_create(Wid, 0) == -1) {
        free(Wid);
        free(Rid);
        ncPrint("pipeOpen --> second sem_create returned -1.");
        return -1;
    }
//    ncPrint("despues semCreatew\n");

    char *buffer = alloc(PIPE_SIZE * sizeof(char));
    if (buffer == 0) {
        free(Wid);
        free(Rid);
        return -1;
    }

    int *nWrite = alloc(sizeof(int));
    int *nRead = alloc(sizeof(int));
    *nRead = 0;
    *nWrite = 0;

    int *rWaiting = alloc(sizeof(int));
    int *wWaiting = alloc(sizeof(int));
    *rWaiting = 0;
    *wWaiting = 0;

//    ncPrint("antes de newPipe\n");
    pipe_t* readPipe = newPipe(READ, lastID++, buffer, nRead, nWrite, Rid, Wid, firstPipe, rWaiting, wWaiting);
    if (readPipe == NULL){
        free(rWaiting);
        free(wWaiting);
        free(nWrite);
        free(nRead);
        free(Wid);
        free(Rid);
        free(buffer);
        ncPrint("No se pudo crear el pipe.");
        return -1;
    }
    pipe_t *writePipe = newPipe(WRITE, lastID++, buffer, nRead, nWrite, Rid, Wid, readPipe, rWaiting, wWaiting);
    if (writePipe == NULL) {
        pipeClose(readPipe->id);
        ncPrint("No se pudo crear el pipe.");
        return -1;
    }
//    ncPrint("despues de newPipe\n");
    firstPipe = writePipe;
    setReverseSide(readPipe, writePipe);
    setReverseSide(writePipe, readPipe);
    array[0] = readPipe->id;
    array[1] = writePipe->id;

    return 0;
}

int pipeWrite(int fd, const char * buffer, int count){
//    ncPrint("A");
    pipe_t * aux;
//    ncPrint("B");
    if (fd==1) {
        aux = getCurrentPCB()->outputPipe;
        if (aux == NULL || aux->type == READ) {//porl
            ncPrint("Error en pipe Write. Pipe no existe o es de lectura.");
            return -1;
        }
        if (aux->buffer == NULL) {      // Salida estandar posta
            for (int i = 0; i < count; ++i) {
                ncPrintChar(buffer[i]);
            }
            return 0;
        }
    } else {
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
            if ((*(aux->read_waiting)) > 0) {
                for (int j = 0; j < (*(aux->read_waiting)); ++j)
                    sem_post(aux->sem_R);
                (*(aux->read_waiting)) = 0;
            }
            (*(aux->write_waiting))++;
            sem_wait(aux->sem_W);
        }
        aux->buffer[(*(aux->nWrite))++ % PIPE_SIZE] = buffer[i];
    }
    if ((*(aux->read_waiting)) > 0) {
        for (int j = 0; j < (*(aux->read_waiting)); ++j)
            sem_post(aux->sem_R);
        (*(aux->read_waiting)) = 0;
    }
    return 0;
}

void freePipe(pipe_t *pipe) {       // NO LIBERA LA ESTRUCTURA DEL PIPE!!!!!
    sem_close(pipe->sem_R);
    sem_close(pipe->sem_W);
    free(pipe->sem_R);
    free(pipe->sem_W);
    free(pipe->buffer);
    free(pipe->nRead);
    free(pipe->nWrite);
    free(pipe->write_waiting);
    free(pipe->read_waiting);
}

int pipeClose(int fd) {
    if (firstPipe == NULL) {
        ncPrint("Pipe not found in pipe close.\n");
        return -1;
    }
    pipe_t *aux = firstPipe->next;
    pipe_t *prev = firstPipe;
    if (prev->id == fd) {
        if (prev->reverse_side == NULL) {
            freePipe(prev);
            firstPipe = prev->next;
            free(prev);
            return 0;
        } else {
            prev->reverse_side->reverse_side = NULL;
            firstPipe = prev->next;
            free(prev);
            return 0;
        }

    }
    while (aux != NULL) {
        if (aux->id == fd)
            break;
        prev = aux;
        aux = aux->next;
    }
    if (aux == NULL) {
        ncPrint("Pipe not found in pipe close.\n");
        return -1;
    }

    if (aux->reverse_side == NULL) {
        prev->next = aux->next;
        freePipe(aux);
        free(aux);
        return 0;
    } else {
        prev->next = aux->next;
        aux->reverse_side->reverse_side = NULL;
        free(aux);
        return 0;
    }


}

int pipeRead(int fd, char * buffer, int count){
//    ncPrint("R1\n");
    pipe_t * aux;
    if(fd==0){
        aux = getCurrentPCB()->inputPipe;
        if (aux->buffer == NULL) {
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
            while ((*(aux->nRead)) == (*(aux->nWrite))){      // TODO:Si otro write me gana, se me solapa lo que escribo
                if((*(aux->write_waiting))>0) {
                    for (int j = 0; j < (*(aux->write_waiting)); ++j)
                        sem_post(aux->sem_W);
                    (*(aux->write_waiting)) = 0;
                }
                (*(aux->read_waiting))++;
                sem_wait(aux->sem_R);
            }
//            ncPrintChar(aux->buffer[(*(aux->nRead)) % PIPE_SIZE]);
//            ncPrintChar(' ');
            buffer[i] = aux->buffer[(*(aux->nRead))++ % PIPE_SIZE];
        }

    if ((*(aux->write_waiting)) > 0) {
        for (int j = 0; j < (*(aux->write_waiting)); ++j)
            sem_post(aux->sem_W);
        (*(aux->write_waiting)) = 0;
    }
    return 0;
}

int dup(char oldId, char id) {
    pipe_t *aux = firstPipe;
    while (aux != NULL) {
        if (aux->id == id)
            break;
        aux = aux->next;
    }
    if (aux == NULL) {
        ncPrint("Error pipe not found in dup\n");
        return -1;
    }
    if (oldId == 0) {
        if (aux->type != READ) {
            ncPrint("Pipe not same type.\n");
            return -1;
        }
        getCurrentPCB()->inputPipe = aux;
        return 0;
    }
    if (oldId == 1) {
        if (aux->type != WRITE) {
            ncPrint("Pipe not same type.\n");
            return -1;
        }
        getCurrentPCB()->outputPipe = aux;
        return 0;
    }
    return -1;
}

pipe_info_wrapper *infoPipe(int *length) {
    *length = 2;
    pipe_t *aux = firstPipe;
    while (aux != NULL) {
        (*length)++;
        aux = aux->next;
    }
    aux = firstPipe;
    pipe_info_wrapper *info = alloc(sizeof(pipe_info_wrapper) * (*length));


    // stdin pipe
    info[0].id = inputP->id;
    info[0].semR = getSemInfo(inputP->sem_R);
    info[0].semW = 0;

    // stdout pipe
    info[1].id = outputP->id;
    info[1].semW = 0;
    info[1].semR = 0;

    // other pipes
    int i = 2;
    while (aux != NULL) {
        info[i].id = aux->id;
        info[i].semR = getSemInfo(aux->sem_R);
        info[i++].semW = getSemInfo(aux->sem_W);
        aux = aux->next;
    }
    return info;
}



