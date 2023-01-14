#ifndef LIGHTSWITCH_H_
#define LIGHTSWITCH_H_

#include "mutex.h"
#include "semaphore.h"

typedef struct Lightswitch {
    int counter;
    Mutex *mutex;
} Lightswitch;

int Lightswitch_init(Lightswitch* lightswitch);

int Lightswitch_lock(Lightswitch* lightswitch, Semaphore *sem);

int Lightswitch_unlock(Lightswitch* lightswitch, Semaphore *sem);

int Lightswitch_destroy(Lightswitch *lightswitch);

#endif // LIGHTSWITCH_H_