
#include "semaphore.h"
#include "include/naiveConsole.h"

static semaphore_t * semaphores = NULL;
int sem_create(char * newId, uint64_t value){
    char * id = alloc((myStrlen(newId)+1)* sizeof(char));
    myStrcpy(id, newId);
        if(semaphores==NULL){
        semaphores = alloc(sizeof(semaphore_t));
        semaphores->id=id;
        semaphores->value = value;
        semaphores->p_waiting=0;
        semaphores->next=NULL;
        semaphores->channel=NULL;
        semaphores->lock = 0;
    } else{
            if(myStrcmp(semaphores->id, id)) {
                ncPrint("Sem already exists.\n");
                ncPrint(id);
                ncPrintChar('\n');
                free(id);
                return -1;
            }

            semaphore_t * aux = semaphores;
            while(aux->next!=NULL){
                if(myStrcmp(aux->id, id)) {
                    ncPrint("Sem already exists.\n");
                    ncPrint(id);
                    ncPrintChar('\n');
                    free(id);
                    return -1;
                }
                aux = aux->next;
            }
            aux->next = alloc(sizeof(semaphore_t));
            aux->next->value = value;
            aux->next->id = id;
            aux->next->p_waiting=0;
            aux->next->next=NULL;
            aux->next->channel=NULL;
            aux->next->lock = 0;
        }
        nSems++;
        return 0;
}

int sem_wait(char * sem_id){
    semaphore_t * semaphore_ptr = semaphores;
    while (semaphore_ptr!=NULL && !myStrcmp(semaphore_ptr->id, sem_id))
        semaphore_ptr = semaphore_ptr->next;
    if(semaphore_ptr==NULL) {
        return -1;
    }


    acquire(&(semaphore_ptr->lock)); // spinlock

    if(semaphore_ptr->value > 0){
        semaphore_ptr->value--;
    } else{
        char process = getCurrentPID();
        if(semaphore_ptr->channel == NULL){
            semaphore_ptr->channel = alloc(sizeof (sem_list_wrapper));
            semaphore_ptr->channel->process = process;
            semaphore_ptr->channel->next = NULL;
        } else{
            sem_list_wrapper * aux = semaphore_ptr->channel;
            while (aux->next!=NULL)
                aux = aux->next;
            aux->next = alloc(sizeof (sem_list_wrapper));
            aux->next->process = process;
            aux->next->next = NULL;
        }
        semaphore_ptr->p_waiting++;
        release(&(semaphore_ptr->lock)); // spinlock        TODO: Agregar la decision de poner este release aca al informe.

        blockProcess();     // Solo lo despierta un post.
        return 0;
    }
    release(&(semaphore_ptr->lock)); // spinlock

    return 0;

}

int sem_post(char * sem_id){
    semaphore_t * semaphore_ptr = semaphores;
    while (semaphore_ptr!=NULL && !myStrcmp(semaphore_ptr->id, sem_id)) {
        semaphore_ptr = semaphore_ptr->next;
    }
    if(semaphore_ptr==NULL) {
        ncPrint("Semaphore not found in sem_post. semId:");
        ncPrint(sem_id);
        ncPrintChar('\n');
        return -1;
    }
    acquire(&(semaphore_ptr->lock)); // spinlock

    if(semaphore_ptr->channel!=NULL){
        char pid = semaphore_ptr->channel->process;
        sem_list_wrapper * aux = semaphore_ptr->channel->next;
        free(semaphore_ptr->channel);
        semaphore_ptr->channel = aux;
        semaphore_ptr->p_waiting--;
        release(&(semaphore_ptr->lock)); // spinlock

        unblockProcessPID(pid);
        return 0;
    } else semaphore_ptr->value++;
    release(&(semaphore_ptr->lock)); // spinlock
    return 0;
}


int sem_close(char * sem_id){
    semaphore_t * semaphore_ptr = semaphores;
    semaphore_t * prev=NULL;
    while (semaphore_ptr!=NULL && !myStrcmp(semaphore_ptr->id, sem_id)) {
        prev = semaphore_ptr;
        semaphore_ptr = semaphore_ptr->next;
    }
    if(semaphore_ptr==NULL) {
        ncPrint("Semaphore not found in sem_close. ID: ");
        ncPrint(sem_id);
        ncPrintChar('\n');
        return -1;
    }


    sem_list_wrapper * aux;
    while (semaphore_ptr->channel!=NULL){
        aux = semaphore_ptr->channel->next;
        free(semaphore_ptr->channel);
        semaphore_ptr->channel = aux;
    }
    free(semaphore_ptr->id);
    if(prev!=NULL){
        prev->next = semaphore_ptr->next;
    } else{
        semaphores = semaphore_ptr->next;
    }
    free(semaphore_ptr);
    nSems--;
    return 0;

}
struct sem_info_wrapper * sem_info(int * qty){
    *qty = nSems;
    sem_info_wrapper * info = alloc(sizeof(sem_info_wrapper)*(*qty));
    semaphore_t * semaphore_ptr = semaphores;
    int j;
    sem_list_wrapper * aux= semaphore_ptr->channel;
    for (int i = 0; i < (*qty); ++i) {
        info[i].id = alloc((myStrlen(semaphore_ptr->id)+1)* sizeof(char));
        myStrcpy((info[i].id), semaphore_ptr->id);
        info[i].value = semaphore_ptr->value;
        info[i].pids = alloc(semaphore_ptr->p_waiting);
        info[i].nPids = semaphore_ptr->p_waiting;
        j=0;
        while (aux!=NULL){
            info[i].pids[j] = aux->process;
            aux = aux->next;
        }
        semaphore_ptr = semaphore_ptr->next;
    }
    return info;
}
