
#include "semaphore.h"
#define STARTING_SEMAPHORES 16
static sem_t * semaphores[] = NULL;
static char sem_counter = 0;
static char sem_array_size = STARTING_SEMAPHORES;
void sem_init(){
    semaphores = alloc(sizeof(sem_t *) * STARTING_SEMAPHORES);
    for (int i = 0; i < STARTING_SEMAPHORES; ++i) {
        semaphores[i] = NULL;
    }
}
int sem_create(uint64_t value){

    if(sem_counter==sem_array_size){
        sem_t * aux[] = semaphores;
        semaphores = alloc(sizeof(sem_t *) * (sem_counter+STARTING_SEMAPHORES));
        sem_array_size += STARTING_SEMAPHORES;
        for (int i = 0; i < sem_array_size; ++i) {
            if(i<sem_counter)
            semaphores[i] = aux[i];
            else semaphores[i] = NULL;
        }
    }

    int auxCount = 0;
        while(1){
            if(semaphores[auxCount]==NULL){
                break;
            }
            auxCount++;
        }
    semaphores[auxCount] = alloc(sizeof(sem_t));
        if(semaphores[auxCount] == NULL)
            return -1;
        semaphores[auxCount]->value = value;
    return auxCount;
}

int sem_wait(int sem_id){
    sem_t * semaphore_ptr = semaphores[sem_id];
    if(semaphore_ptr==NULL)
        return -1;
    // TODO: Implement spinlocks
    // acquire(semaphore_ptr->lock) // spinlock
    if(semaphore_ptr->value > 0){
        semaphore_ptr->value--;
    } else{
        // release(semaphore_ptr->lock) // spinlock
        PCB * process = getCurrentPCB();
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
        //sleep(process);     // Lo despierta solo un post // TODO: Sleep
    }

}

int sem_post(int sem_id){
    sem_t * semaphore_ptr = semaphores[sem_id];
    if(semaphore_ptr==NULL)
        return -1;
    // acquire(semaphore_ptr->lock) // spinlock
    if(semaphore_ptr->channel!=NULL){
        PCB * process = semaphore_ptr->channel->process;
        semaphore_ptr->channel = semaphore_ptr->channel->next;
        semaphore_ptr->p_waiting--;
//        wakeup(process);  // TODO: Wakeup
    } else semaphore_ptr->value++;
    // release(semaphore_ptr->lock) // spinlock
}

int sem_close(int sem_id){
    sem_t * semaphore_ptr = semaphores[sem_id];
    if(semaphore_ptr==NULL)
        return -1;
    // acquire(semaphore_ptr->lock) // spinlock
    // TODO: Wakeup all processes?
    // release(semaphore_ptr->lock) // spinlock
    sem_list_wrapper * aux;
    while (semaphore_ptr->channel!=NULL){
        aux = semaphore_ptr->channel->next;
        free(semaphore_ptr->channel);
        semaphore_ptr->channel = aux;
    }
    free(semaphore_ptr);
    semaphores[sem_id] = NULL;
}

