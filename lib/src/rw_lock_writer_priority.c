#include "../include/rw_lock_writer_priority.h"

#include <stdlib.h>
#include <stdio.h>

int RW_Lock_Writer_Priority_init(RW_Lock_Writer_Priority* rw_lock){
    if (rw_lock == NULL){
        printf("Can not initialize null rwlocker!\n");
        return -1;
    }
    
    rw_lock->read_switch = (Lightswitch *) malloc(sizeof(Lightswitch));
    Lightswitch_init(rw_lock->read_switch);
    rw_lock->write_switch = (Lightswitch *) malloc(sizeof(Lightswitch));
    Lightswitch_init(rw_lock->write_switch);
    rw_lock->no_readers = (Semaphore *) malloc(sizeof(Semaphore));
    Semaphore_init(rw_lock->no_readers, 1);
    rw_lock->no_writers = (Semaphore *) malloc(sizeof(Semaphore));
    Semaphore_init(rw_lock->no_writers, 1);

    return 0;
}

int RW_Lock_Writer_Priority_wait(RW_Lock_Writer_Priority* rw_lock, int mode){
    if (rw_lock == NULL){
        printf("Can not wait on null rwlocker!\n");
        return -1;
    }
    
    if(mode == 1){ // Writer Mode
        Lightswitch_lock(rw_lock->write_switch, rw_lock->no_readers);
        Semaphore_wait(rw_lock->no_writers);
    } else if(mode == 0){ // Reader Mode  
        Semaphore_wait(rw_lock->no_readers);
        Lightswitch_lock(rw_lock->read_switch, rw_lock->no_writers);
        Semaphore_signal(rw_lock->no_readers);
    } else {
        printf("Invalid mode for rw_lock");
        return -1;
    }

    return 0;
}

int RW_Lock_Writer_Priority_signal(RW_Lock_Writer_Priority* rw_lock, int mode){
    if (rw_lock == NULL){
        printf("Can not signal to null rwlocker!\n");
        return -1;
    }
    
    if(mode == 1){ // Writer Mode
        Semaphore_signal(rw_lock->no_writers);
        Lightswitch_unlock(rw_lock->write_switch, rw_lock->no_readers);
    } else if(mode == 0){ // Reader Mode
        Lightswitch_unlock(rw_lock->read_switch, rw_lock->no_writers);
    } else {
        printf("Invalid mode for rw_lock");
        return -1;
    }

    return 0;
}

int RW_Lock_Writer_Priority_destroy(RW_Lock_Writer_Priority* rw_lock){
    if (rw_lock == NULL){
        printf("Can not destroy null rwlocker!\n");
        return -1;
    }
    
    Lightswitch_destroy(rw_lock->read_switch);
    Lightswitch_destroy(rw_lock->write_switch);
    Semaphore_destroy(rw_lock->no_readers);
    Semaphore_destroy(rw_lock->no_writers);

    return 0;
}
