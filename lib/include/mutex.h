#ifndef MUTEX_H_
#define MUTEX_H_

#include "thread_queue.h"

#include <stdbool.h>

typedef struct Mutex {
    bool locked;
    bool guard;
    Thread_Queue* thread_queue;
} Mutex;

int Mutex_init(Mutex* mtx);

int Mutex_acquire(Mutex* mtx);

int Mutex_release(Mutex* mtx);

int Mutex_destroy(Mutex* mtx);

#endif // MUTEX_H_