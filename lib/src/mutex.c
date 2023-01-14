#include "../include/mutex.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void sig_handler(int signal) {
    // Signal is caught, thread is woken up.
    return;
}

int Mutex_init(Mutex* mtx){
    if (mtx == NULL){
        printf("Can not initialize null mtx!\n");
        return -1;
    }
    signal(SIGUSR1, sig_handler);
    mtx->guard = false;
    mtx->locked = false;
    mtx->thread_queue = malloc(sizeof(Thread_Queue));
    Queue_init(mtx->thread_queue);

    return 0;
}

int Mutex_acquire(Mutex* mtx){
    if (mtx == NULL){
        printf("Can not lock null mtx!\n");
        return -1;
    }

    while (true){
        while (true)
            if (__sync_val_compare_and_swap(&(mtx->guard), false, true) == false)
                break;

        if (__sync_val_compare_and_swap(&(mtx->locked), false, true) == false){
            __sync_val_compare_and_swap(&(mtx->guard), true, false);
            return 0;
        }

        pthread_t* pthread = malloc(sizeof(pthread_t));
        *pthread = pthread_self();
        Queue_push(mtx->thread_queue, pthread);
        __sync_val_compare_and_swap(&(mtx->guard), true, false);
        pause();
    }

    return 0;
}

int Mutex_release(Mutex* mtx){
    if (mtx == NULL){
        printf("Can not unlock null mtx!\n");
        return -1;
    }
    while (true)
        if (__sync_val_compare_and_swap(&(mtx->guard), false, true) == false)
            break;

    pthread_t* pthread = Queue_pop(mtx->thread_queue);
    if (pthread != NULL)
        pthread_kill(*pthread, SIGUSR1);

    __sync_val_compare_and_swap(&(mtx->guard), true, false);
    __sync_val_compare_and_swap(&(mtx->locked), true, false);

    return 0;
}

int Mutex_destroy(Mutex* mtx){
    if (mtx == NULL){
        printf("Can not destroy null mtx!\n");
        return -1;
    }
    Queue_clear(mtx->thread_queue);

    return 0;
}
