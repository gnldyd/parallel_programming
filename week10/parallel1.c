/*
 * 2021 1st semester - Parallel Computing - Assignment 8
 * 202172213 Hwiyong Chang
 * $ mpicc -o run.o parallel1.c
 * $ mpiexec -n 2 run.o
 */

#include <stdio.h>
#include <mpi.h>

int main(void) {
    int comm_sz;
    int my_rank;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    #define n 4
    int x[n] = {1, 2, 3, 4};

    // ---Algorithm-----
    int sum = 0, partial_sums[n];
    int *sum_p = &sum; 
    for (int index = 0; index < my_rank + 1; ++index) {
        sum += x[index];
    }
    MPI_Send(sum_p, 1, MPI_INT, 0, my_rank, MPI_COMM_WORLD);
    if (my_rank == 0) {
        for (int index = 0; index < n; ++index) {
            MPI_Recv(sum_p, 1, MPI_INT, index, index, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            partial_sums[index] = sum;
        }
    }
    // -----------------

    if (my_rank == 0) {
        for (int index = 0; index < n; ++index) {
            printf("prefix sums %d is %d\n", index + 1, partial_sums[index]);
        }
    }

    MPI_Finalize();
    return 0;
}