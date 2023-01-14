#include "../include/semaphore.h"

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

int Semaphore_init(Semaphore* sem, int val){
    if (sem == NULL){
        printf("Can not initialize null semaphore!\n");
        return -1;
    }

    sem->mutex = (Mutex*) malloc(sizeof(Mutex));
    Mutex_init(sem->mutex);

    sem->value = val;
    sem->thread_queue = (Thread_Queue*) malloc(sizeof(Thread_Queue));
    Queue_init(sem->thread_queue);
    
    sem->signal_set = (sigset_t *) malloc(sizeof(sigset_t));
    sigemptyset(sem->signal_set);
    sigaddset(sem->signal_set, SIGUSR1);

    return 0;
}

int Semaphore_wait(Semaphore* sem){
    if (sem == NULL){
        printf("Can not wait on null semaphore!\n");
        return -1;
    }
    
    Mutex_acquire(sem->mutex);
    sem->value --;
    if(sem->value < 0){
        pthread_t* pthread = (pthread_t*) malloc(sizeof(pthread_t));
        *pthread = pthread_self();
        Queue_push(sem->thread_queue, pthread);
        Mutex_release(sem->mutex);

        int sig;
        sigwait(sem->signal_set, &sig);
    } else {
        Mutex_release(sem->mutex);
    }

    return 0;
}

int Semaphore_signal(Semaphore* sem){
    if (sem == NULL){
        printf("Can not signal to null semaphore!\n");
        return -1;
    }

    Mutex_acquire(sem->mutex);
    sem->value ++;
    if(sem->value <= 0){
        pthread_t* pthread = Queue_pop(sem->thread_queue);
        pthread_kill(*pthread, SIGUSR1);
    }
    Mutex_release(sem->mutex);

    return 0;
}

int Semaphore_destroy(Semaphore* sem){
    if (sem == NULL){
        printf("Can not destroy null semaphore!\n");
        return -1;
    }
    
    Mutex_destroy(sem->mutex);
    Queue_clear(sem->thread_queue);
    free(sem->signal_set);

    return 0;
}