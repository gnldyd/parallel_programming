/*
 * 2021 1st semester - Parallel Computing - Assignment 9
 * 202172213 Hwiyong Chang
 * $ gcc -o run.o remainder.c -lpthread
 * $ ./run.o 7
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int thread_count;
double sum;
int n, flag;
pthread_mutex_t mutex;

void *Thread_sum(void *rank) {
    long my_rank = (long)rank;
    double factor;
    long long i;
    long long my_n = n / (thread_count - 1);
    long long my_first_i = my_rank == thread_count - 1 ? (my_n * (thread_count - 1)) : my_n * my_rank;
    long long my_last_i = my_rank == thread_count - 1 ? n : my_first_i + my_n;

    factor = my_first_i % 2 == 0 ? 1.0 : -1.0;

    for (i = my_first_i; i < my_last_i; ++i, factor *= -1) {
        pthread_mutex_lock(&mutex);
        sum += factor / (2 * i + 1);
        pthread_mutex_unlock(&mutex);
    }
    printf("my_rank = %ld, my_first_i = %lld, my_last_i = %lld\n", my_rank, my_first_i, my_last_i);
}

int main(int argc, char *argv[]) {
    long thread;
    pthread_t *thread_handles;

    thread_count = strtol(argv[1], NULL, 10);
    sum = 0.0; n = 100000; flag = 0;
    thread_handles = malloc(thread_count * sizeof(pthread_t));

    for (thread = 0; thread < thread_count; ++thread) {
        pthread_create(&thread_handles[thread], NULL, Thread_sum, (void*) thread);
    }

    for (thread = 0; thread < thread_count; ++thread) {
        pthread_join(thread_handles[thread], NULL);
    }
    printf("sum = %lf\n", 4 * sum);
    free(thread_handles);
    return 0;
}