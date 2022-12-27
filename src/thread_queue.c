#include "../include/thread_queue.h"

#include <stdio.h>
#include <stdlib.h>

void Queue_init(Thread_Queue* q){
    q = malloc(sizeof(Thread_Queue));
    q->front = NULL;
    q->back = NULL;
    q->count = 0;
}

void Queue_clear(Thread_Queue* q){
    if (q == NULL){
        return;
    }
    TQ_Node* curr_node;
    TQ_Node* next_node;
    for (curr_node = q->front; curr_node != NULL; curr_node = next_node){
        next_node = curr_node->next;
        Free_node(curr_node);
    }
    free(q);
}

void Queue_push(Thread_Queue* q, pthread_t* thr){
    if (q == NULL){
        printf("Can not perform push operation to uninitialized queue!\n");
        exit(-1);
    }

    TQ_Node* node = malloc(sizeof(TQ_Node));
    node->next = NULL;
    node->thread = thr;

    if (q->front != NULL){
        q->back->next = node;
        q->back = node;
        q->count++;
    }
    else {
        q->front = node;
        q->back = node;
        q->count = 1;
    }
}

void Free_node(TQ_Node* node){
    free(node->thread);
    free(node);
}

pthread_t* Queue_front(Thread_Queue* q){
    if (q == NULL){
        printf("Can not perform front operation on uninitialized queue!\n");
        exit(-1);
    }
    
    return q->front;
}

pthread_t* Queue_pop(Thread_Queue* q){
    if (q == NULL){
        printf("Can not perform pop operation on uninitialized queue!\n");
        exit(-1);
    }
    if (q->front == NULL){
        return NULL;
    }

    TQ_Node* result = q->front;
    q->front = q->front->next;
    q->count--;
    if (q->count == 0){
        q->back = NULL;
    }

    return result->thread;
}