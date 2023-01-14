#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include "../lib/include/lockers.h"

#define CNT_PHILOSOPHERS 5

int cnt_philosophers = CNT_PHILOSOPHERS;

char *meals[] = {"breakfast", "lunch", "dinner"};
Semaphore *forks;
Semaphore footman;

void* philosopher_routine(void*);

int get_left_fork(int);
int get_right_fork(int);
void get_forks(int);
void put_forks(int);
void think(int, int);
void eat(int, int);

int main(int argc, char *argv[]){
   
    pthread_t *tids = (pthread_t *)malloc(cnt_philosophers * sizeof(pthread_t));
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    forks = (Semaphore *) malloc(cnt_philosophers * sizeof(Semaphore));
    for(int i = 0; i < cnt_philosophers; i ++)
        Semaphore_init(forks + i, 1);
    Semaphore_init(&footman, cnt_philosophers - 1);

    for(int i = 0; i < cnt_philosophers; i ++){
        int *philosopher_id = (int *)malloc(sizeof(int));
        *philosopher_id = i;
        if(pthread_create(tids + i - 1, &attr, philosopher_routine, philosopher_id)){
            perror(NULL);
            return errno;
        }
    }

    for(int i = 0; i < cnt_philosophers; i ++)
        if(pthread_join(tids[i], NULL)){
            perror(NULL);
            return errno;
        }

    free(tids);
    for(int i = 0; i < cnt_philosophers; i ++)
        Semaphore_destroy(forks + i);
    free(forks);
    Semaphore_destroy(&footman);

    return 0;
}


void think(int philosopher_id, int i){
    for(int j = 1; j <= 1000000; j ++);
    printf("Philosopher %d is thinking before %s\n", philosopher_id, meals[i]);
}

void eat(int philosopher_id, int i){
    for(int j = 1; j <= 1000000; j ++);
    printf("Philosopher %d is eating %s\n", philosopher_id, meals[i]);
}

int get_left_fork(int i){
    return i;
}

int get_right_fork(int i){
    return (i + 1) % 5;
}

void get_forks(int i){
    Semaphore_wait(&footman);
    Semaphore_wait(forks + get_right_fork(i));
    Semaphore_wait(forks + get_left_fork(i));
}

void put_forks(int i){
    Semaphore_signal(forks + get_right_fork(i));
    Semaphore_signal(forks + get_left_fork(i));
    Semaphore_signal(&footman);
}

void* philosopher_routine(void* params){
    int philosopher_id = *((int *)params);
    for(int i = 0; i < 3; i ++){
        think(philosopher_id, i);
        get_forks(philosopher_id);
        eat(philosopher_id, i);
        put_forks(philosopher_id);
    }
}