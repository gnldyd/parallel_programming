/*
 * 2021 1st semester - Parallel Computing - Assignment 6
 * 202172213 Hwiyong Chang
 * $ mpicc -o run.o mpi_output.c
 * $ mpiexec -n 4 run.o
 */

#include <stdio.h>
#include <mpi.h>

int main(void) {
    int my_rank, comm_sz;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if (my_rank > 0) {
        MPI_Recv(NULL, 0, MPI_PACKED, my_rank - 1, my_rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
    }  // 0 이상의 rank를 가진 프로세스는 이전 rank의 응답을 기다린다.
    printf("Proc %d of %d > Does anyone have a toothpick?\n", my_rank, comm_sz);
    if (my_rank < comm_sz - 1) {
        MPI_Send(NULL, 0, MPI_PACKED, my_rank + 1, my_rank + 1, MPI_COMM_WORLD);
    }  // 마지막 rank를 제외한 프로세스는 다음 rank에게 빈 메시지를 전달한다.

    MPI_Finalize();
    return 0;
}
