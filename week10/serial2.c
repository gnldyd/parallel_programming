/*
 * 2021 1st semester - Parallel Computing - Assignment 8
 * 202172213 Hwiyong Chang
 * $ gcc -o run.o serial2.c -lm
 * $ ./run.o
 */

#include <stdio.h>
#include <math.h>

int main(void) {
    #define n 4
    #define k (int)log2(n)
    int x[n] = {1, 2, 3, 4};

    // ---Algorithm-----
    int original[n], partial_sums[n];
    for (int index = 0; index < n; ++index) {
        partial_sums[index] = x[index];
        original[index] = partial_sums[index]; // copy 
    }
    
    for(int k_index = 0; k_index < k; ++k_index) {
        int period = (int)pow(2, k_index);
        for(int index = 0; index < n; ++index) {
            if (index >= period) {
                partial_sums[index] += original[index - period];
            }
        } // partial_sums[index] + partial_sums[index - period]

        for(int index = 0; index < n; ++index) {
            original[index] = partial_sums[index];
        } // update original
    }
    // -----------------

    for (int index = 0; index < n; ++index) {
        printf("prefix sums %d is %d\n", index + 1, partial_sums[index]);
    }
    return 0;
}