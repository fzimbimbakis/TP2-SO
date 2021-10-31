
#include "semaphore.h"
#include "include/naiveConsole.h"

//#define STARTING_SEMAPHORES 16
static semaphore_t * semaphores = NULL;
//static char sem_counter = 0;
//static char sem_array_size = STARTING_SEMAPHORES;
//void sem_init(){
//    semaphores = alloc(sizeof(semaphore_t *) * STARTING_SEMAPHORES);
//    for (int i = 0; i < STARTING_SEMAPHORES; ++i) {
//        semaphores[i] = NULL;
//    }
//}
//int sem_open(char * sem_id){
//    sem_t * semaphore_ptr = semaphores;
//    while (semaphore_ptr!=NULL && !myStrcmp(semaphore_ptr->id, sem_id))
//        semaphore_ptr = semaphore_ptr->next;
//    if(semaphore_ptr==NULL) {
//        return -1;
//    }
//    semaphore_ptr->nOpen++;
//}
int sem_create(char * newId, uint64_t value){
//      Array semaphores
//    if(sem_counter==sem_array_size){
//        semaphore_t * aux[] = semaphores;
//        semaphores = alloc(sizeof(semaphore_t *) * (sem_counter+STARTING_SEMAPHORES));
//        sem_array_size += STARTING_SEMAPHORES;
//        for (int i = 0; i < sem_array_size; ++i) {
//            if(i<sem_counter)
//            semaphores[i] = aux[i];
//            else semaphores[i] = NULL;
//        }
//        free(aux);
//    }
//    int auxCount = 0;
//        while(1){
//            if(semaphores[auxCount]==NULL){
//                break;
//            }
//            auxCount++;
//        }
//    semaphores[auxCount] = alloc(sizeof(semaphore_t));
//        if(semaphores[auxCount] == NULL)
//            return -1;
//        semaphores[auxCount]->value = value;
    //      Linked list semaphores
    char * id = alloc(myStrlen(newId)* sizeof(char));
    myStrcpy(id, newId);
        if(semaphores==NULL){
        semaphores = alloc(sizeof(semaphore_t));
        semaphores->id=id;
        semaphores->value = value;
        semaphores->p_waiting=0;
        semaphores->next=NULL;
        semaphores->channel=NULL;
        semaphores->lock = 0;
//        semaphores->nOpen = 0;
    } else{
            if(myStrcmp(semaphores->id, id))
                return -1;

            semaphore_t * aux = semaphores;
            while(aux->next!=NULL){
                if(myStrcmp(aux->id, id))
                    return -1;
                aux = aux->next;
            }
            aux->next = alloc(sizeof(semaphore_t));
            aux->next->value = value;
            aux->next->id = id;
            aux->next->p_waiting=0;
            aux->next->next=NULL;
            aux->next->channel=NULL;
            aux->next->lock = 0;
//            aux->next->nOpen=0;
        }
        nSems++;
        return 0;
}

int sem_wait(char * sem_id){
//    semaphore_t * semaphore_ptr = semaphores[sem_id];

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
//    semaphore_t * semaphore_ptr = semaphores[sem_id];
    semaphore_t * semaphore_ptr = semaphores;
    while (semaphore_ptr!=NULL && !myStrcmp(semaphore_ptr->id, sem_id))
        semaphore_ptr = semaphore_ptr->next;
    if(semaphore_ptr==NULL)
        return -1;
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
    if(semaphore_ptr==NULL)
        return -1;


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
        info[i].id = alloc(myStrlen(semaphore_ptr->id)* sizeof(char));
        myStrcpy((info[i].id), semaphore_ptr->id);
        info[i].value = semaphore_ptr->value;
        info[i].pids = alloc(semaphore_ptr->p_waiting);
        info[i].nPids = semaphore_ptr->p_waiting;
        ncPrintDec(info[i].nPids);
        j=0;
        while (aux!=NULL){
            info[i].pids[j] = aux->process;
            aux = aux->next;
        }
    }
    return info;
}
