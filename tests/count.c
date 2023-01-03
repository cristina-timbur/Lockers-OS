#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include "../lockers.h"

#define MAX_RESOURCES 5

int available_resources = MAX_RESOURCES;

int decrease_count(int);
int increase_count(int);
void* consumer(void*);

Mutex mtx;

int main(int argc, char *argv[]){
    printf("MAX_RESOURCES = %d\n", MAX_RESOURCES);
    
    pthread_t *tids = (pthread_t *)malloc((argc - 1) * sizeof(pthread_t));
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    if(Mutex_init(&mtx) == -1){
        return -1;
    }

    for(int i = 1; i < argc; i ++)
        if(pthread_create(tids + i - 1, &attr, consumer, argv[i])){
            perror(NULL);
            return errno;
        }

    for(int i = 1; i < argc; i ++)
        if(pthread_join(tids[i - 1], NULL)){
            perror(NULL);
            return errno;
        }

    free(tids);
    Mutex_destroy(&mtx);

    return 0;
}

int decrease_count(int count){
    Mutex_acquire(&mtx);
    if(available_resources < count){
        Mutex_release(&mtx);
        printf("Tried to get %d resources only %d available\n", count, available_resources);
        return -1;
    }
    available_resources -= count;
    printf("Got %d resources %d remaining\n", count, available_resources);
    Mutex_release(&mtx);
    return 0;
}

int increase_count(int count){
    Mutex_acquire(&mtx);
    available_resources += count;
    printf("Released %d resources %d remaining\n", count, available_resources);
    Mutex_release(&mtx);
    return 0; 
}

void* consumer(void *params){
    int count = atoi((char *)params);
    while(decrease_count(count) == -1);
    increase_count(count);
}