/*
 * 2021 1st semester - Parallel Computing - Assignment 8
 * 202172213 Hwiyong Chang
 * $ mpicc -o run.o parallel2.c -lm
 * $ mpiexec -n 4 run.o
 */

#include <stdio.h>
#include <mpi.h>
#include <math.h>

int main(void) {
    int comm_sz;
    int my_rank;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    #define n 4
    #define k (int)log2(n)
    int x[n] = {1, 2, 3, 4};

    // ---Algorithm-----
    int partial_sums[n];
    partial_sums[my_rank] = x[my_rank];
    for(int k_index = 0; k_index < k; ++k_index) {
        int period = (int)pow(2, k_index);
        if (my_rank < n - period) {    // send
            MPI_Send(&partial_sums[my_rank], 1, MPI_INT, my_rank + period, my_rank + period, MPI_COMM_WORLD);
        }
        if (my_rank >= period) {       // receive
            int buffer;
            MPI_Recv(&buffer, 1, MPI_INT, my_rank - period, my_rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            partial_sums[my_rank] += buffer;
        }
    }
    MPI_Send(&partial_sums[my_rank], 1, MPI_INT, 0, my_rank, MPI_COMM_WORLD);
    // -----------------

    if (my_rank == 0) {
        for (int index = 0; index < n; ++index) {
            MPI_Recv(&partial_sums[index], 1, MPI_INT, index, index, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("prefix sums %d is %d\n", index + 1, partial_sums[index]);
        }
    }

    MPI_Finalize();
    return 0;
}