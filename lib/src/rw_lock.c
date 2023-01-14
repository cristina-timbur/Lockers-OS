#include "../include/rw_lock.h"

#include <stdlib.h>
#include <stdio.h>

int RW_Lock_init(RW_Lock* rw_lock){
    if (rw_lock == NULL){
        printf("Can not initialize null rwlocker!\n");
        return -1;
    }

    rw_lock->room_empty = (Semaphore *) malloc(sizeof(Semaphore));
    Semaphore_init(rw_lock->room_empty, 1);
    rw_lock->turnstile = (Semaphore *) malloc(sizeof(Semaphore));
    Semaphore_init(rw_lock->turnstile, 1);
    rw_lock->read_switch = (Lightswitch *) malloc(sizeof(Lightswitch));
    Lightswitch_init(rw_lock->read_switch);

    return 0;
}

int RW_Lock_wait(RW_Lock* rw_lock, int mode){
    if (rw_lock == NULL){
        printf("Can not wait on null rwlocker!\n");
        return -1;
    }

    if(mode == 1) { // Writer Mode
        Semaphore_wait(rw_lock->turnstile);
        Semaphore_wait(rw_lock->room_empty);
    } else if(mode == 0){ // Reader Mode
        Semaphore_wait(rw_lock->turnstile);
        Semaphore_signal(rw_lock->turnstile);

        Lightswitch_lock(rw_lock->read_switch, rw_lock->room_empty);
    } else {
        return -1;
    }

    return 0;
}

int RW_Lock_signal(RW_Lock* rw_lock, int mode){
    if (rw_lock == NULL){
        printf("Can not signal to null rwlocker!\n");
        return -1;
    }
    
    if(mode == 1){ // Writer Mode
        Semaphore_signal(rw_lock->turnstile);
        Semaphore_signal(rw_lock->room_empty);
    } else if(mode == 0){ // Reader Mode
        Lightswitch_unlock(rw_lock->read_switch, rw_lock->room_empty);
    } else {
        printf("Invalid mode for rw_lock");
        return -1;
    } 

    return 0;
}

int RW_Lock_destroy(RW_Lock* rw_lock){
    if (rw_lock == NULL){
        printf("Can not destroy null rwlocker!\n");
        return -1;
    }
    
    Semaphore_destroy(rw_lock->room_empty);
    Semaphore_destroy(rw_lock->turnstile);
    Lightswitch_destroy(rw_lock->read_switch);

    return 0;
}