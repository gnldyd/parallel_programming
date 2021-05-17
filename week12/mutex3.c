/*
 * 2021 1st semester - Parallel Computing - Assignment 10
 * 202172213 Hwiyong Chang
 * $ gcc -o run.o mutex3.c -lpthread
 * $ ./run.o 1000
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int thread_count;
pthread_mutex_t mutex;
int available = 0;
#define n 100
char message[n];

void *thread_message(void *rank) {
    long my_rank = (long)rank;

    while (1) {
        pthread_mutex_lock(&mutex);
        if(available == my_rank) {
            if (my_rank > 0) {
                printf("%s", message);
            }
            if (my_rank < thread_count - 1) {
                sprintf(message, "Thread %ld > This message from thread %ld to thread %ld.\n", my_rank+1, my_rank, my_rank+1);
            }
            ++available;
            pthread_mutex_unlock(&mutex);
            break;
        }
        pthread_mutex_unlock(&mutex);
    }
}

int main(int argc, char *argv[]) {
    long thread;
    pthread_t *thread_handles;

    thread_count = strtol(argv[1], NULL, 10);
    thread_handles = malloc(thread_count * sizeof(pthread_t));

    pthread_mutex_init(&mutex, NULL);
    for (thread = 0; thread < thread_count; ++thread) {
        pthread_create(&thread_handles[thread], NULL, thread_message, (void*) thread);
    }
    
    for (thread = 0; thread < thread_count; ++thread) {
        pthread_join(thread_handles[thread], NULL);
    }
    pthread_mutex_destroy(&mutex);
    free(thread_handles);
    return 0;
}