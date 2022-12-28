#include "../include/rw_lock_writer_priority.h"

#include <stdlib.h>
#include <stdio.h>

void RW_Lock_Writer_Priority_init(RW_Lock_Writer_Priority* rw_lock){
    rw_lock = (RW_Lock_Writer_Priority *) malloc(sizeof(RW_Lock_Writer_Priority));
    Lightswitch_init(rw_lock->read_switch);
    Lightswitch_init(rw_lock->write_switch);
    Semaphore_init(rw_lock->no_readers);
    Semaphore_init(rw_lock->no_writers);
}

void RW_Lock_Writer_Priority_wait(RW_Lock_Writer_Priority* rw_lock, int mode){
    if(mode == 1){ // Writer Mode
        Semaphore_wait(rw_lock->no_readers);
        Lightswitch_lock(rw_lock->read_switch, rw_lock->no_writers);
        Semaphore_signal(rw_lock->no_readers);
    } else if(mode == 0){ // Reader Mode  
        Lightswitch_lock(rw_lock->write_switch, rw_lock->readers);
        Semaphore_wait(rw_lock->no_writers);
    } else {
        printf("Invalid mode for rw_lock");
        exit(-1);
    }
}

void RW_Lock_Writer_Priority_signal(RW_Lock_Writer_Priority* rw_lock, int mode){
    if(mode == 1){ // Writer Mode
        Lightswitch_unlock(rw_lock->read_switch, rw_lock->no_writers)
    } else if(mode == 0){ // Reader Mode
        Semaphore_signal(rw_lock->no_writers);
        Lightswitch_unlock(rw_lock->write_switch, rw_lock->no_readers);
    } else {
        printf("Invalid mode for rw_lock");
        exit(-1);
    }
}

void RW_Lock_Writer_Priority_destroy(RW_Lock_Writer_Priority* rw_lock){
    Lightswitch_destroy(rw_lock->read_switch);
    Lightswitch_destroy(rw_lock->write_switch);
    Semaphore_destroy(rw_lock->no_readers);
    Semaphore_destroy(rw_lock->no_writers);
    free(rw_lock);
}
