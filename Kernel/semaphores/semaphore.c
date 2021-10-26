
#include "semaphore.h"
#define STARTING_SEMAPHORES 16
static sem_t * semaphores = NULL;
static char sem_counter = 0;
static char sem_array_size = STARTING_SEMAPHORES;
void sem_init(){
//    semaphores = alloc(sizeof(sem_t *) * STARTING_SEMAPHORES);
//    for (int i = 0; i < STARTING_SEMAPHORES; ++i) {
//        semaphores[i] = NULL;
//    }
}
int sem_create(char * id, uint64_t value){
//      Array semaphores
//    if(sem_counter==sem_array_size){
//        sem_t * aux[] = semaphores;
//        semaphores = alloc(sizeof(sem_t *) * (sem_counter+STARTING_SEMAPHORES));
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
//    semaphores[auxCount] = alloc(sizeof(sem_t));
//        if(semaphores[auxCount] == NULL)
//            return -1;
//        semaphores[auxCount]->value = value;
    //      Linked list semaphores
        if(semaphores==NULL){
        semaphores = alloc(sizeof(sem_t));
        semaphores->id=id;
        semaphores->value = value;
    } else{
            if(myStrcmp(semaphores->id, sem_id))
                return -1;

            sem_t * aux = semaphores;
            while(aux->next!=NULL){
                if(myStrcmp(aux->id, sem_id))
                    return -1;
                aux = aux->next;
            }
            aux->next = alloc(sizeof(sem_t));
            aux->next->value = value;
            aux->next->id = id;
        }
        return 0;
}

int sem_wait(char * sem_id){
//    sem_t * semaphore_ptr = semaphores[sem_id];

    sem_t * semaphore_ptr = semaphores;
    while (semaphore_ptr!=NULL && myStrcmp(semaphore_ptr->id, sem_id))
        semaphore_ptr = semaphore_ptr->next;
    if(semaphore_ptr==NULL)
        return -1;

    // TODO: Implement spinlocks
    // acquire(semaphore_ptr->lock) // spinlock
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

        blockProcess();     // Solo lo despierta un post.
    }
    // release(semaphore_ptr->lock) // spinlock

}

int sem_post(char * sem_id){
//    sem_t * semaphore_ptr = semaphores[sem_id];
    sem_t * semaphore_ptr = semaphores;
    while (semaphore_ptr!=NULL && myStrcmp(semaphore_ptr->id, sem_id))
        semaphore_ptr = semaphore_ptr->next;
    if(semaphore_ptr==NULL)
        return -1;
    // acquire(semaphore_ptr->lock) // spinlock
    if(semaphore_ptr->channel!=NULL){
        char pid = semaphore_ptr->channel->process;
        sem_list_wrapper * aux = semaphore_ptr->channel->next;
        free(semaphore_ptr->channel);
        semaphore_ptr->channel = aux;
        semaphore_ptr->p_waiting--;
        unblockProcess(pid);
    } else semaphore_ptr->value++;
    // release(semaphore_ptr->lock) // spinlock
}

int sem_close(int sem_id){

    sem_t * semaphore_ptr = semaphores;
    sem_t * prev=NULL;
    while (semaphore_ptr!=NULL && myStrcmp(semaphore_ptr->id, sem_id)) {
        prev = semaphore_ptr;
        semaphore_ptr = semaphore_ptr->next;
    }
    if(semaphore_ptr==NULL)
        return -1;

    // acquire(semaphore_ptr->lock) // spinlock
    // TODO: Wakeup all processes?
    sem_list_wrapper * aux;
    while (semaphore_ptr->channel!=NULL){
        aux = semaphore_ptr->channel->next;
        free(semaphore_ptr->channel);
        semaphore_ptr->channel = aux;
    }
    if(prev!=NULL){
        prev->next = semaphore_ptr->next;
    }
    free(semaphore_ptr);
    // release(semaphore_ptr->lock) // spinlock
}

