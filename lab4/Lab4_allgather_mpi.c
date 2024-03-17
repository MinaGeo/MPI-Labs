// https://www.mpich.org/static/docs/v3.1.3/www3/MPI_Gather.html
// A collective communication tool, unlike the point-to-point Send/Recv
// Gathers together (concatenates) values from all other processes into root

#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

int main() {

    MPI_Init(NULL, NULL);
    int world_size, my_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);


    // Initialized for all processes to their rank
    int my_value [2] = {my_rank, my_rank+1};

    // Pointer to store the gathered data from other processes; init to 0
    int* gathered_values = (int*) malloc((world_size * sizeof(int))*2);
    for (int i = 0; i < world_size*2; i++) gathered_values[i] = 0;

    // Each process prints its current value
    printf(" Before gather: #%d has values = [ %d %d ]\n", my_rank, my_value[0], my_value[1]);

    // Process #0 gathers values from other processes, while they send them
    // MPI_Gather(sendbuf, sendcount, sendtype,
    //            recvbuf, recvcount, recvtype, <-- significant only at root
    //            root, MPI_Comm)
    MPI_Allgather(
        my_value, 2, MPI_INT,
        gathered_values, 2, MPI_INT, // <-- significant only at root
        MPI_COMM_WORLD
    );

    // Each process prints its current value and gathered values
    printf("  After gather: #%d has value = [ %d %d ]\n", my_rank, my_value[0],my_value[1]);
    printf("   And gathered values = ");
    for (int i = 0; i < world_size*2; i++) printf("%d ", gathered_values[i]);
    
    

    printf("\n\n"); // I must make the output look clean!!

    MPI_Finalize();

    return 0;

}





