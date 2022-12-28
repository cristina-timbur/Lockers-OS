#include "../include/rw_lock.h"

#include <stdlib.h>
#include <stdio.h>

void RW_Lock_init(RW_Lock* rw_lock){
    rw_lock = (RW_Lock*) malloc(sizeof(RW_Lock));

    Semaphore_init(rw_lock->room_empty, 1);
    Semaphore_init(rw_lock->turnstile, 1);
    Lightswitch_init(rw_lock->read_switch);
}

void RW_Lock_wait(RW_Lock* rw_lock, int mode){
    if(mode == 1) { // Writer Mode
        Semaphore_wait(rw_lock->turnstile);
        Semaphore_wait(rw_lock->room_empty);
    } else if(mode == 0){ // Reader Mode
        Semaphore_wait(rw_lock->turnstile);
        Semaphore_signal(rw_lock->turnstile);

        Lightswitch_lock(rw_lock->read_switch, rw_lock->room_empty);
    } else {
        printf("Invalid mode for rw_lock");
        exit(-1);
    }
}

void RW_Lock_signal(RW_Lock* rw_lock, int mode){
    if(mode == 1){ // Writer Mode
        Semaphore_signal(rw_lock->turnstile)
        Semaphore_signal(rw_lock->room_empty);
    } else if(mode == 0){ // Reader Mode
        Lightswitch_unlock(rw_lock->read_switch, rw_lock->room_empty);
    } else {
        printf("Invalid mode for rw_lock");
        exit(-1);
    } 
}

void RW_Lock_destroy(RW_Lock* rw_lock){
    Semaphore_destroy(rw_lock->room_empty);
    Semaphore_destroy(rw_lock->turnstile);
    Lightswitch_destroy(rw_lock->read_switch);
    free(rw_lock);
}