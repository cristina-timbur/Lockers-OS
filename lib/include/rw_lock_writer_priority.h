#ifndef RW_LOCK_WRITER_PRIORITY_H_
#define RW_LOCK_WRITER_PRIORITY_H_

#include "lightswitch.h"
#include "semaphore.h"

typedef struct RW_Lock_Writer_Priority{
    Lightswitch* read_switch;
    Lightswitch* write_switch;
    Semaphore *no_readers;
    Semaphore *no_writers;
} RW_Lock_Writer_Priority;

int RW_Lock_Writer_Priority_init(RW_Lock_Writer_Priority* rw_lock);

int RW_Lock_Writer_Priority_wait(RW_Lock_Writer_Priority* rw_lock, int mode);

int RW_Lock_Writer_Priority_signal(RW_Lock_Writer_Priority* rw_lock, int mode);

int RW_Lock_Writer_Priority_destroy(RW_Lock_Writer_Priority* rw_lock);

#endif // RW_LOCK_WRITER_PRIORITY_H