#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
//All credits to Eng/ Ahmad Salama 😁
#define N 1//32000

int main(int argc, char const* argv[]) {

    int world_size, my_rank;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    /*Normally intializing
    */

    if (my_rank == 0) {

        int* a = (int*) malloc(sizeof(int) * N);
        for (int i = 1; i < world_size; i++)
            MPI_Recv(a, N, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    } else {

        int* a = (int*) malloc(sizeof(int) * N);
        // MPI_Send(a, N, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Ssend(&a, N, MPI_INT, 0, 0, MPI_COMM_WORLD); //used insteal of MPI_barrier. it will block until the recipient has received the message
        /* meaninig we will get results arranged not puzzeled
        int MPI_Ssend(const void *buf, int count, MPI_Datatype datatype, int dest,
              int tag, MPI_Comm comm)*/
        // What about non-blocking communication?
        printf("Rank %d/%d done!\n", my_rank, world_size);
        fflush(stdout);

    }

    MPI_Finalize();
    return 0;

}
