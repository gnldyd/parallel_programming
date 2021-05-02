/*
 * 2021 1st semester - Parallel Computing - Assignment 7
 * 202172213 Hwiyong Chang
 * $ mpicc -o run.o pingpong_202172213.c
 * $ mpiexec -n 2 run.o
 */

#include <stdio.h>
#include <time.h>
#include <mpi.h>

void pingpong_Cclock(int rank, int rally_count) {
    // opponent_rank
    int opponent_rank = rank == 0 ? 1 : 0;
    int rally_data = 1;  // rally index data
    int *buffer; // buffer for rally_data
    buffer = &rally_data;

    // C clock time measure
    clock_t ct_start, ct_end;

    if (rank == 0) { // serve process
        for (; rally_data <= rally_count; ++rally_data) {
            ct_start = clock();
            MPI_Send(buffer, 1, MPI_INT, opponent_rank, rally_data, MPI_COMM_WORLD);
            MPI_Recv(buffer, 1, MPI_INT, opponent_rank, rally_data, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            ct_end = clock();
            printf("Ping-pong: rally %d, C_clock Time is %f seconds.\n", rally_data, (double)(ct_end - ct_start) / CLOCKS_PER_SEC);
        }
    } else if (rank == 1) { // receive process
        for (; rally_data <= rally_count; ++rally_data) {
            MPI_Recv(buffer, 1, MPI_INT, opponent_rank, rally_data, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(buffer, 1, MPI_INT, opponent_rank, rally_data, MPI_COMM_WORLD);
        }
    }
}

void pingpong_MPIWtime(int rank, int rally_count) {
    // opponent_rank
    int opponent_rank = rank == 0 ? 1 : 0;
    int rally_data = 1; // rally index data
    int *buffer; // buffer for rally_data
    buffer = &rally_data;

    // MPI time measure
    double mt_start, mt_end;

    if (rank == 0) { // serve process
        for (; rally_data <= rally_count; ++rally_data) {
            mt_start = MPI_Wtime();
            MPI_Send(buffer, 1, MPI_INT, opponent_rank, rally_data, MPI_COMM_WORLD);
            MPI_Recv(buffer, 1, MPI_INT, opponent_rank, rally_data, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            mt_end = MPI_Wtime();
            printf("Ping-pong: rally %d, MPI Time is %f seconds.\n", rally_data, mt_end - mt_start);
        }
    } else if (rank == 1) { // receive process
        for (; rally_data <= rally_count; ++rally_data) {
            MPI_Recv(buffer, 1, MPI_INT, opponent_rank, rally_data, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(buffer, 1, MPI_INT, opponent_rank, rally_data, MPI_COMM_WORLD);
        }
    }
}

int main(void) {
    int my_rank, comm_sz;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    int rally_count = 10;
    pingpong_Cclock(my_rank, rally_count); // C clock time
    pingpong_MPIWtime(my_rank, rally_count); // MPI time
    printf("comm_sz = %d\n", comm_sz);

    MPI_Finalize();
    return 0;
}