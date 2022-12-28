#ifndef THREAD_QUEUE_H_
#define THREAD_QUEUE_H_

#include <pthread.h>

typedef struct __TQNode TQ_Node;

struct __TQNode {
    pthread_t* thread;
    TQ_Node* next;
};

typedef struct Thread_Queue{
    TQ_Node* front;
    TQ_Node* back;
    int count;
} Thread_Queue;

void Queue_init(Thread_Queue* q);

void Queue_clear(Thread_Queue* q);

void Queue_push(Thread_Queue* q, pthread_t* thr);

void Free_node(TQ_Node* TQ_Node);

pthread_t* Queue_front(Thread_Queue* q);

pthread_t* Queue_pop(Thread_Queue* q);

int Queue_empty(Thread_Queue* q);

#endif // THREAD_QUEUE_H_