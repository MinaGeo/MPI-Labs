// https://www.mpich.org/static/docs/v3.1.3/www3/MPI_Barrier.html
// A collective communication tool, unlike the point-to-point Send/Recv
//All credits to Eng/Ahmad Salama
// Blocks until all processes in the communicator have reached this routine.

#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

#define N 1e8

int main(int argc, char const* argv[]) {

    int world_size, my_rank;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if (my_rank != 0) {

        int* arr = malloc(sizeof(int) * N); //allocating a memory for array
        for (int i = 0; i < N; i++) arr[i] = rand(); //filling the array with random numbers
        // XXX
        // MPI_Send(NULL, 0, 0, 0, 0, MPI_COMM_WORLD);
        //int MPI Barrier(MPI Comm comm /∗ in ∗/); 
        MPI_Barrier(MPI_COMM_WORLD);
        /*  MPI Barrier ensures that no process will return from calling it until every process in the communicator
            has started calling it*/
    } else {

        // https://www.mpich.org/static/docs/v3.2/www3/MPI_Wtime.html
        double start_time = MPI_Wtime(); //This function measures time at current time (like Millis ely fel arduino)
        // XXX
        // for (int i = 1; i < world_size; i++) {
        //     char a;
        //     MPI_Recv(NULL, 0, 0, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        // }
        MPI_Barrier(MPI_COMM_WORLD);
        double end_time = MPI_Wtime();
        printf("\nTime taken to initialize array:\n\
            % fsec\n\n\n\n", end_time - start_time);

    }
    MPI_Finalize();

    return 0;
}
