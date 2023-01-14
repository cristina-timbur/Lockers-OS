#include "../include/lightswitch.h"

#include <stdlib.h>
#include <stdio.h>

int Lightswitch_init(Lightswitch* lightswitch){
    if (lightswitch == NULL){
        printf("Can not initialize null lightswitch!\n");
        return -1;
    }

    lightswitch->counter = 0;
    lightswitch->mutex = (Mutex*) malloc(sizeof(Mutex)); 
    Mutex_init(lightswitch->mutex);

    return 0;
}


int Lightswitch_lock(Lightswitch* lightswitch, Semaphore *sem){
    if (lightswitch == NULL){
        printf("Can not lock null lightswitch!\n");
        return -1;
    }
    
    Mutex_acquire(lightswitch->mutex);
    lightswitch->counter ++;
    if(lightswitch->counter == 1){
        Semaphore_wait(sem);
    }
    Mutex_release(lightswitch->mutex);

    return 0;
}

int Lightswitch_unlock(Lightswitch* lightswitch, Semaphore *sem){
    if (lightswitch == NULL){
        printf("Can not unlock null lightswitch!\n");
        return -1;
    }
    
    Mutex_acquire(lightswitch->mutex);
    lightswitch->counter --;
    if(lightswitch->counter == 0){
        Semaphore_signal(sem);
    }
    Mutex_release(lightswitch->mutex);

    return 0;
}

int Lightswitch_destroy(Lightswitch *lightswitch){
    if (lightswitch == NULL){
        printf("Can not destroy null lightswitch!\n");
        return -1;
    }
    
    Mutex_destroy(lightswitch->mutex);

    return 0;
}