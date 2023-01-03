#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include "thread_queue.h"
#include "mutex.h"

#include <signal.h>

typedef struct Semaphore {
    int value;
    Mutex *mutex;
    Thread_Queue *thread_queue;
    sigset_t* signal_set;
} Semaphore;

int Semaphore_init(Semaphore* sem, int val);

int Semaphore_wait(Semaphore* sem);

int Semaphore_signal(Semaphore* sem);

int Semaphore_destroy(Semaphore* sem);

#endif // SEMAPHORE_H_