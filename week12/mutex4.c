/*
 * 2021 1st semester - Parallel Computing - Assignment 10
 * 202172213 Hwiyong Chang
 * $ gcc -o run.o mutex4.c -lpthread
 * $ ./run.o 2
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int thread_count;
pthread_mutex_t mutex0, mutex1;
int time_count = 0;

void *thread_deadlock(void *rank) {
    long my_rank = (long)rank;

    if (my_rank == 0) {
        pthread_mutex_lock(&mutex0);
        ++time_count;
        while(time_count / thread_count < 1);
        pthread_mutex_lock(&mutex1);
        printf("Thread %ld > work.\n", my_rank);
        pthread_mutex_unlock(&mutex1);
        pthread_mutex_unlock(&mutex0);
    }
    else if(my_rank == 1) {
        pthread_mutex_lock(&mutex1);
        ++time_count;
        while(time_count / thread_count < 1);
        pthread_mutex_lock(&mutex0);
        printf("Thread %ld > work.\n", my_rank);
        pthread_mutex_unlock(&mutex0);
        pthread_mutex_unlock(&mutex1);
    }
}

int main(int argc, char *argv[]) {
    long thread;
    pthread_t *thread_handles;

    thread_count = strtol(argv[1], NULL, 10);
    thread_handles = malloc(thread_count * sizeof(pthread_t));

    pthread_mutex_init(&mutex0, NULL);
    pthread_mutex_init(&mutex1, NULL);
    for (thread = 0; thread < thread_count; ++thread) {
        pthread_create(&thread_handles[thread], NULL, thread_deadlock, (void*) thread);
    }
    
    for (thread = 0; thread < thread_count; ++thread) {
        pthread_join(thread_handles[thread], NULL);
    }
    pthread_mutex_destroy(&mutex0);
    pthread_mutex_destroy(&mutex1);
    free(thread_handles);
    return 0;
}