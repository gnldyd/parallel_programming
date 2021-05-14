/*
 * 2021 1st semester - Parallel Computing - Assignment 9
 * 202172213 Hwiyong Chang
 * $ gcc -o run.o semaphore2.c -lpthread
 * $ ./run.o 1000
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

int thread_count;
double sum;
int n, flag;
sem_t semaphore;

void *Thread_sum(void *rank) {
    long my_rank = (long)rank;
    double factor;
    long long i;
    long long my_n = n / thread_count;
    long long my_first_i = my_n * my_rank;
    long long my_last_i = my_first_i + my_n;
    double my_sum = 0.0;

    factor = my_first_i % 2 == 0 ? 1.0 : -1.0;

    for (i = my_first_i; i < my_last_i; ++i, factor *= -1) {
        my_sum += factor / (2 * i + 1);
    }
    sem_post(&semaphore);
    sum += my_sum;
    sem_wait(&semaphore);
}

int main(int argc, char *argv[]) {
    long thread;
    pthread_t *thread_handles;

    thread_count = strtol(argv[1], NULL, 10);
    sum = 0.0; n = 60000; flag = 0;
    thread_handles = malloc(thread_count * sizeof(pthread_t));

    sem_init(&semaphore, 0, 1);
    for (thread = 0; thread < thread_count; ++thread) {
        pthread_create(&thread_handles[thread], NULL, Thread_sum, (void*) thread);
    }
    
    for (thread = 0; thread < thread_count; ++thread) {
        pthread_join(thread_handles[thread], NULL);
    }
    sem_destroy(&semaphore);
    printf("sum = %lf\n", 4 * sum);
    free(thread_handles);
    return 0;
}