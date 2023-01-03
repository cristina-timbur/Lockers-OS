#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <semaphore.h>
#include <stdlib.h>
#include "../lockers.h"

RW_Lock_Writer_Priority rw_lock;
int value = 0;

void* reader_func(void*);
void* writer_func(void*);

int main(int argc, char *argv[]){
    int readers = 0;
    int writers = 0;
    
    pthread_t *tids = (pthread_t *)malloc((argc - 1) * sizeof(pthread_t));
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    
    if(RW_Lock_Writer_Priority_init(&rw_lock) == -1){
        return -1;
    }

    for(int i = 1; i < argc; i ++){
        if(*argv[i] == 'r'){
            readers ++;
            int *reader_id = (int *)malloc(sizeof(int));
            *reader_id = readers;
            if(pthread_create(tids + i - 1, &attr, reader_func, reader_id)){
                perror(NULL);
                return errno; 
            }
        }

        if(*argv[i] == 'w'){
            writers ++;
            int *writer_id = (int *)malloc(sizeof(int));
            *writer_id = writers;
            if(pthread_create(tids + i - 1, &attr, writer_func, writer_id)){
                perror(NULL);
                return errno; 
            }
        }
    }

    for(int i = 1; i < argc; i ++)
        if(pthread_join(tids[i - 1], NULL)){
            perror(NULL);
            return errno;
        }

    free(tids);
    RW_Lock_Writer_Priority_destroy(&rw_lock);

    return 0;
}

void* reader_func(void *params){
    int reader_id = *((int *)params);
    RW_Lock_Writer_Priority_wait(&rw_lock, 0);
    printf("Reader %d found value %d\n", reader_id, value);
    RW_Lock_Writer_Priority_signal(&rw_lock, 0);
}

void* writer_func(void *params){
    int writer_id = *((int *)params);
    RW_Lock_Writer_Priority_wait(&rw_lock, 1);
    printf("Writer %d found value %d and updated it to ", writer_id, value);
    value += writer_id;
    printf("%d\n", value);
    RW_Lock_Writer_Priority_signal(&rw_lock, 1);
}
