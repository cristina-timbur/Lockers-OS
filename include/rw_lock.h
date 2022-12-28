#ifndef RW_LOCK_H_
#define RW_LOCK_H_

#include "mutex.h"
#include "semaphore.h"
#include "lightswitch.h";

typedef struct RW_Lock{
    Semaphore *room_empty;
    Semaphore *turnstile;
    Lightswitch *read_switch;
} RW_Lock;

void RW_Lock_init(RW_Lock* rw_lock);

void RW_Lock_wait(RW_Lock* rw_lock, int mode);

void RW_Lock_signal(RW_Lock* rw_lock, int mode);

void RW_Lock_destroy(RW_Lock* rw_lock);

#endif // RW_LOCK_H