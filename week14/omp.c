/*
 * 2021 1st semester - Parallel Computing - Assignment 12
 * 202172213 Hwiyong Chang
 * $ gcc -fopenmp -o run.o omp.c
 * $ ./run.o 2
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

float local_sum(float *a, int start, int end, int n) {
    int my_rank = omp_get_thread_num();
    int thread_count = omp_get_num_threads();

    int local_n = n / thread_count;
    int local_start = start + my_rank * local_n;
    int local_end = local_start + local_n - 1;

    float local_sum = 0.0;

    for (; local_start <= local_end; ++local_start)
        local_sum += a[local_start];
    return local_sum;
}

int main(int argc, char *argv[]) {
    float a[] = {4.0, 3.0, 3.0, 1000.0};
    int i;
    float sum = 0.0;

    #pragma omp parallel for num_threads(2) \
        reduction(+:sum)
    for (i = 0; i < 4; i++)
        sum += a[i];

    // #pragma omp parallel num_threads(2) reduction(+:sum)
    // sum += local_sum(a, 0, 4, 4);

    printf("sum = %4.lf\n", sum);
    return 0;
}