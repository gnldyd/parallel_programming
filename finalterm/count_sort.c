/*
 * 2021 1st semester - Parallel Computing - final exam
 * 202172213 Hwiyong Chang
 * $ gcc -fopenmp -o run.o count_sort.c
 * $ ./run.o 4
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>


void print_array(int array[], int n, int head) {
    printf("[%d", array[0]);
    for (int i = 1; i < head; ++i)
        printf(", %d", array[i]);
    printf(", ... ");
    for (int i = n - head; i < n; ++i)
        printf(", %d", array[i]);
    printf("]\n");
}


int compare(const void *n1, const void *n2) {
    return *(int *)n1 < *(int *)n2 ? -1 : *(int *)n1 > *(int *)n2 ? 1 : 0;
}


void Count_sort(int a[], int n, int thread_count) {
    int i, j, count;
    int* temp = malloc(n * sizeof(int));
    #pragma omp parallel for num_threads(thread_count) \
        private(i, j, count) shared(a, n)
    for (i = 0; i < n; i++) {
        count = 0;
        for(j = 0; j < n; j++)
            if (a[j] < a[i])
                count++;
            else if (a[j] == a[i] && j < i)
                count++;
        temp[count] = a[i];
    }
    // memcpy(a, temp, n * sizeof(int));
    int quotient = n / thread_count;
    #pragma omp parallel num_threads(thread_count)
    memcpy(a + omp_get_thread_num() * quotient, temp + omp_get_thread_num() * quotient, sizeof(int) * quotient);
    free(temp);
}


int main(int argc, char *argv[]) {
    srand(time(NULL));  // random seed
    #define N 60000
    int a[N], b[N], thread_count, head = 5;
    sscanf(argv[1],"%d", &thread_count);
    clock_t start_c, end_c, start_q, end_q;
    for (int i = 0; i < N; ++i)
        a[i] = b[i] = rand() % (2 << 30);  // 0 ~ 2147483647
    
    printf("Before:\na = ");
    print_array(a, N, 5);

    start_c = clock();
    Count_sort(a, N, thread_count);
    end_c = clock();

    printf("After:\na = ");
    print_array(a, N, head);
    printf("Time of Count_sort is %.4f seconds.\n", (float)(end_c - start_c)/CLOCKS_PER_SEC);

    printf("Before:\nb = ");
    print_array(b, N, head);

    start_q = clock();
    qsort(b, N, sizeof(int), compare);
    end_q = clock();

    printf("After:\nb = ");
    print_array(b, N, head);
    printf("Time of qsort is %.4f seconds.\n", (float)(end_q - start_q)/CLOCKS_PER_SEC);
    return 0;
}