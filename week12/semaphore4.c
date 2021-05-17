/*
 * 2021 1st semester - Parallel Computing - Assignment 10
 * 202172213 Hwiyong Chang
 * $ gcc -o run.o semaphore4.c -lpthread
 * $ ./run.o 2
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

int thread_count;
int time_count = 0;
sem_t semaphore0, semaphore1;

void *thread_deadlock(void *rank) {
    long my_rank = (long)rank;

    if (my_rank == 0) {
        sem_post(&semaphore0);
        ++time_count;
        while(time_count / thread_count < 1);
        sem_post(&semaphore1);
        printf("Thread %ld > work.\n", my_rank);
        sem_wait(&semaphore1);
        sem_wait(&semaphore0);
    }
    else if(my_rank == 1) {
        sem_post(&semaphore1);
        ++time_count;
        while(time_count / thread_count < 1);
        sem_post(&semaphore0);
        printf("Thread %ld > work.\n", my_rank);
        sem_wait(&semaphore0);
        sem_wait(&semaphore1);
    }
}

int main(int argc, char *argv[]) {
    long thread;
    pthread_t *thread_handles;

    thread_count = strtol(argv[1], NULL, 10);
    thread_handles = malloc(thread_count * sizeof(pthread_t));

    sem_init(&semaphore0, 0, 1);
    sem_init(&semaphore1, 0, 1);
    for (thread = 0; thread < thread_count; ++thread) {
        pthread_create(&thread_handles[thread], NULL, thread_deadlock, (void*) thread);
    }
    
    for (thread = 0; thread < thread_count; ++thread) {
        pthread_join(thread_handles[thread], NULL);
    }
    sem_destroy(&semaphore0);
    sem_destroy(&semaphore1);
    free(thread_handles);
    return 0;
}