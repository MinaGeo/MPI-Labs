// https://www.mpich.org/static/docs/v3.1.3/www3/MPI_Reduce.html
// https://www.mpich.org/static/docs/latest/www3/Constants.html
// A collective communication tool, unlike the point-to-point Send/Recv
// "Reduces" values on all processes to a single value at the "root" process

#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
//To fix intellisense for VS Code, add "${MSMPI_INC}" to the include path in the workspace settings
//To compile, use: gcc 1.1-hello-mpi.c -I $env:MSMPI_INC\ -L $env:MSMPI_LIB64\ -lmsmpi
//To execute, use: mpiexec -n M a.exe [this for M processes (replace M with number)]

int main() {

    MPI_Init(NULL, NULL);
    int world_size, my_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // Initialized for all processes to their rank + 9
    int value[] = {my_rank, my_rank+1, my_rank+2};

 // Each process prints its current value
    printf(" Before reduce: #%d, array value = [ %d %d %d ", my_rank, value[0], value[1], value[2]);
    printf("]\n");

    // All processes invoke reduction for "root" process #0 with SUM reduction operation
    // MPI_Reduce(sendbuf, recvbuf (significant only at root), count, datatype, MPI_Op, root, MPI_Comm)
    int recvbuf []= {0,0,0};
    MPI_Reduce(&value, &recvbuf, 3, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

        // Each process prints its current value
    printf(" after reduce: #%d, array value = [ %d %d %d ", my_rank, value[0], value[1], value[2]);
        printf("]\n");
        printf(" recvbuf is now: [ %d %d %d ", recvbuf[0], recvbuf[1], recvbuf[2]);
     printf("]\n");
    printf("*---------------------------------------------*\n");

 
    MPI_Finalize();
    return 0;

}
