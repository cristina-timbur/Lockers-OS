#ifndef LIGHTSWITCH_H_
#define LIGHTSWITCH_H_

#include "mutex.h"
#include "semaphore.h"

typedef struct Lightswitch {
    int counter;
    Mutex *mutex;
} Lightswitch;

void Lightswitch_init(Lightswitch* lightswitch);

void Lightswitch_lock(Lightswitch* lightswitch, Semaphore *sem);

void Lightswitch_unlock(Lightswitch* lightswitch, Semaphore *sem);

void Lightswitch_destroy(Lightswitch *lightswitch);

#endif // LIGHTSWITCH_H_