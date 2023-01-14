#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <semaphore.h>
#include <stdlib.h>
#include "../lib/include/lockers.h"

#define NTHRS 5

int nthrs = NTHRS;

struct barrier{
    Mutex *mtx;
    Semaphore *sem;
    int count;
    int lim;
} barr;

void *tfun(void*);
int barrier_init(struct barrier*, int);
void barrier_destroy(struct barrier*);
void barrier_point(struct barrier*);

int main(){
    printf("NTHRS = %d\n", NTHRS);

    if(barrier_init(&barr, nthrs))
        return errno;

    pthread_t *tids = (pthread_t *)malloc(nthrs * sizeof(pthread_t));
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    for(int i = 0; i < nthrs; i ++){
        int *p = (int *)malloc(sizeof(int));
        *p = i;
        if(pthread_create(tids + i, &attr, tfun, p)){
            perror(NULL);
            return errno;
        }
    }

    for(int i = 0; i < nthrs; i ++)
        if(pthread_join(tids[i], NULL)){
            perror(NULL);
            return errno;
        }

    free(tids);
    barrier_destroy(&barr);

    return 0;
}

int barrier_init(struct barrier* b, int n){
    b->mtx = (Mutex*) malloc(sizeof(Mutex));
    b->sem = (Semaphore*) malloc(sizeof(Semaphore));
    if(Mutex_init(b->mtx) == -1){
        return -1;
    }
    if(Semaphore_init(b->sem, 0)){
        return -1;
    }
    b->count = 0;
    b->lim = n;
    return 0;
}

void barrier_destroy(struct barrier* b){
    Mutex_destroy(b->mtx);
    Semaphore_destroy(b->sem);
    free(b->mtx);
    free(b->sem);
}

void *tfun(void *v){
    int *tid = (int *)v;

    printf("%d reached the barrier\n", *tid);
    barrier_point(&barr);
    printf("%d passed the barrier\n", *tid);

    free(tid);
    return NULL;
}

void barrier_point(struct barrier* b){
    Mutex_acquire(b->mtx);
    b->count ++;
    if(b->count == b->lim)
        if(Semaphore_signal(b->sem) == -1){
            return;
        }
    Mutex_release(b->mtx);

    if(Semaphore_wait(b->sem) == -1){
        return;
    }

    if(Semaphore_signal(b->sem) == -1){
        return;
    }
}


