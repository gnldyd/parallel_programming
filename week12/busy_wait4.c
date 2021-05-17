/*
 * 2021 1st semester - Parallel Computing - Assignment 10
 * 202172213 Hwiyong Chang
 * $ gcc -o run.o busy_wait4.c -lpthread
 * $ ./run.o 2
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int thread_count;
int time_count;
int access0 = 1, access1 = 1;

void *thread_deadlock(void *rank) {
    long my_rank = (long)rank;

    if (my_rank == 0) {
        access0 = 0;
        ++time_count;
        while(time_count / thread_count < 1);
        access1 = 0;
        while(access0 != 1 && access1 != 1);
        printf("Thread %ld > work.\n", my_rank);
        access1 = 1;
        access0 = 1;
    }
    else if(my_rank == 1) {
        access1 = 0;
        ++time_count;
        while(time_count / thread_count < 1);
        access0 = 0;
        while(access0 != 1 && access1 != 1);
        printf("Thread %ld > work.\n", my_rank);
        access0 = 1;
        access1 = 1;
    }
}

int main(int argc, char *argv[]) {
    long thread;
    pthread_t *thread_handles;

    thread_count = strtol(argv[1], NULL, 10);
    thread_handles = malloc(thread_count * sizeof(pthread_t));

    for (thread = 0; thread < thread_count; ++thread) {
        pthread_create(&thread_handles[thread], NULL, thread_deadlock, (void*) thread);
    }
    
    for (thread = 0; thread < thread_count; ++thread) {
        pthread_join(thread_handles[thread], NULL);
    }
    free(thread_handles);
    return 0;
}