#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include "../lib/include/lockers.h"

#define BUF_SIZE 5

int buf_sz = BUF_SIZE;

struct buffer {
    int* q;
    Mutex* mtx;
    Semaphore *items;
    Semaphore *spaces;
    int first, last, buf_sz;
} buf;

void buffer_init(struct buffer*, int);
void buffer_destroy(struct buffer*);
void buffer_add(struct buffer*, int);
int buffer_get(struct buffer*);

void* producer(void*);
void* consumer(void*);

int main(int argc, char *argv[]){
    printf("BUF_SIZE = %d\n", BUF_SIZE);

    int producers = 0;
    int consumers = 0;

    time_t t;
    srand((unsigned) time(&t));
    
    pthread_t *tids = (pthread_t *)malloc((argc - 1) * sizeof(pthread_t));
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    buffer_init(&buf, buf_sz);

    for(int i = 1; i < argc; i ++){
        if(*argv[i] == 'p'){
            producers ++;
            int *producer_id = (int *)malloc(sizeof(int));
            *producer_id = producers;
            if(pthread_create(tids + i - 1, &attr, producer, producer_id)){
                perror(NULL);
                return errno; 
            }
        }

        if(*argv[i] == 'c'){
            consumers ++;
            int *consumer_id = (int *)malloc(sizeof(int));
            *consumer_id = consumers;
            if(pthread_create(tids + i - 1, &attr, consumer, consumer_id)){
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
    buffer_destroy(&buf);

    return 0;
}

void buffer_init(struct buffer* buf, int sz){
    buf->mtx = (Mutex*) malloc(sizeof(Mutex));
    buf->items = (Semaphore*)malloc(sizeof(Semaphore));
    buf->spaces = (Semaphore*)malloc(sizeof(Semaphore));

    Mutex_init(buf->mtx);
    Semaphore_init(buf->items, 0);
    Semaphore_init(buf->spaces, sz);

    buf->q = (int *)malloc(sz * sizeof(int));
    buf->first = 0;
    buf->last = 0;
    buf->buf_sz = sz;
}

void buffer_destroy(struct buffer* buf){
    Mutex_destroy(buf->mtx);
    Semaphore_destroy(buf->items);
    Semaphore_destroy(buf->spaces);
    free(buf->mtx);
    free(buf->items);
    free(buf->spaces);
    free(buf->q);
}

void buffer_add(struct buffer* buf, int val){
    *(buf->q + buf->last) = val;
    buf->last = (buf->last + 1) % buf_sz;
}

int buffer_get(struct buffer* buf){
    int x = *(buf->q + buf->first);
    buf->first= (buf->first + 1) % buf_sz;
    return x;
}


void* consumer(void *params){
    int consumer_id = *((int *)params);

    Semaphore_wait(buf.items);
    Mutex_acquire(buf.mtx);

    int x = buffer_get(&buf);
    printf("Consumer %d got value %d from buffer\n", consumer_id, x);
    
    Mutex_release(buf.mtx);
    Semaphore_signal(buf.spaces);

    for(int i = 1; i <= 1000000; i ++);
    printf("Consumer %d done processing\n", consumer_id);
}

void* producer(void *params){
    int producer_id = *((int *)params);

    for(int i = 1; i <= 1000000; i ++);

    Semaphore_wait(buf.spaces);
    Mutex_acquire(buf.mtx);

    int x = rand() % 100;
    buffer_add(&buf, x);
    printf("Producer %d added value %d to buffer\n", producer_id, x);

    Mutex_release(buf.mtx);
    Semaphore_signal(buf.items);
}
