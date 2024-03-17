// https://www.mpich.org/static/docs/v3.1.3/www3/MPI_Scatter.html
// A collective communication tool, unlike the point-to-point Send/Recv
// Splits data from one process across all other processes in a communicator

#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

int main() {

    MPI_Init(NULL, NULL);
    int world_size, my_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // Initialized for all processes to their rank
    int my_value [3] = {my_rank, my_rank+1, my_rank+2};

    // Process with rank #0 initializes a bunch of values of its own
    int* values;
    if (my_rank == 0) {
        values = (int*) malloc(sizeof(int) * (world_size)*3);
        for (int i = 0; i < world_size * 3; i++) {
            values[i] = 420 + i;
        }
    }

    // Each process prints its current value
    printf(" Before scatter: #%d has value = [ %d %d %d ]\n", my_rank, my_value[0],my_value[1],my_value[2]);

    // Process #0 scatters its values while everyone else collects
    // MPI_Scatter(sendbuf, sendcount, sendtype, <-- significant only at root
    //             recvbuf, recvcount, recvtype,
    //             root, MPI_Comm)
    MPI_Scatter(
        values, 3, MPI_INT, // <-- significant only at root
        &my_value, 3, MPI_INT,
        0, MPI_COMM_WORLD
    );

    // Each process prints its current value
    printf("  After scatter: #%d has value = [ %d %d %d ]\n", my_rank, my_value[0],my_value[1],my_value[2]);

    MPI_Finalize();
    return 0;

}
