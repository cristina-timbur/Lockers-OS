#include "../include/lightswitch.h"

#include <stdlib.h>

void Lightswitch_init(Lightswitch* lightswitch){
    lightswitch = (Lightswitch *) malloc(sizeof(Lightswitch));
    lightswitch->counter = 0;
    Mutex_init(lightswitch->mutex);
}


void Lightswitch_lock(Lightswitch* lightswitch, Semaphore *sem){
    Mutex_acquire(lightswitch->mutex);
    lightswitch->counter ++;
    if(lightswitch->counter == 1){
        Semaphore_wait(sem);
    }
    Mutex_release(lightswitch->mutex);
}

void Lightswitch_unlock(Lightswitch* lightswitch, Semaphore *sem){
    Mutex_acquire(lightswitch->mutex);
    lightswitch->counter --;
    if(lightswitch->counter == 0){
        Semaphore_signal(sem);
    }
    Mutex_release(lightswitch->mutex);
}

void Lightswitch_destroy(Lightswitch *lightswitch){
    Mutex_destroy(lightswitch->mutex);
    free(lightswitch);
}