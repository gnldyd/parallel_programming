/*
 * 2021 1st semester - Parallel Computing - Assignment 8
 * 202172213 Hwiyong Chang
 * $ gcc -o run.o serial1.c
 * $ ./run.o
 */

#include <stdio.h>

int main(void) {
    #define n 4
    int x[n] = {1, 2, 3, 4};

    // ---Algorithm-----
    int sum = 0, partial_sums[n];
    for (int index = 0; index < n; ++index) {
        sum += x[index];
        partial_sums[index] = sum;
    }
    // -----------------

    for (int index = 0; index < n; ++index) {
        printf("prefix sums %d is %d\n", index + 1, partial_sums[index]);
    }
    return 0;
}