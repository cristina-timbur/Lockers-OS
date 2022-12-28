#include "../include/semaphore.h"

#include <signal.h>
#include <stdlib.h>

void Semaphore_init(Semaphore* sem, int val){
    sem = (Semaphore *) malloc(sizeof(Semaphore));
    Mutex_init(sem->mutex);

    sem->value = val;
    Queue_init(sem->thread_queue);
    
    sem->signal_set = (sigset_t *) malloc(sizeof(sigset_t));
    sigemptyset(sem->signal_set);
    sigaddset(sem->signal_set, SIGUSR1);
}

void Semaphore_wait(Semaphore* sem){
    Mutex_acquire(sem->mutex);
    sem->value --;
    if(sem->value < 0){
        Queue_push(sem->thread_queue, pthread_self());
        Mutex_release(sem->mutex);
        sigwait(sem->signal_set, SIGUSR1);
    }
}

void Semaphore_signal(Semaphore* sem){
    Mutex_acquire(sem->mutex);
    sem->value ++;
    if(sem->value <= 0){
        pthread_t pthread = Queue_pop(sem->thread_queue);
        pthread_kill(pthread, SIGUSR1);
    }
    Mutex_release(sem->mutex);
}

void Semaphore_destroy(Semaphore* sem){
    Mutex_destroy(sem->mutex);
    Queue_clear(sem->thread_queue);
    free(sem->signal_set);
    free(sem);
}