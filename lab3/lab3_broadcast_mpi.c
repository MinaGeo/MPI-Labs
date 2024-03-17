#include <string.h>
#include <stdio.h>
#include <mpi.h>

//To fix intellisense for VS Code, add "${MSMPI_INC}" to the include path in the workspace settings
//To compile, use: gcc 1.1-hello-mpi.c -I $env:MSMPI_INC\ -L $env:MSMPI_LIB64\ -lmsmpi
//To execute, use: mpiexec -n M a.exe [this for M processes (replace M with number)]

#define MAX_STRING 100

int main(int argc, char* argv[]) {
    int world_size, my_rank;
    //Initialize the MPI execution environment
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    int value[] = {my_rank,(my_rank+1),(my_rank+2)};
    
    if(my_rank==0)
    {
        value[0] = 200;
        value[1] = 300;
        value[2] = 400;
    }
    // if(my_rank != 0){
    for(int i = 0; i<3; i++)
    {
        // Each process prints its current value
    printf(" Before bcast: #%d has on position #%d, value = %d\n", my_rank, i, value[i]);

    }
    printf("*--------------------------------------*\n");
    // Process #0 broadcasts its current value, everyone else listens
    // MPI_Bcast(buffer, count, datatype, root, MPI_Comm)
    MPI_Bcast(&value, 3, MPI_INT, 0, MPI_COMM_WORLD);

    for(int i = 0; i<3; i++)
    {
        // Each process prints its current value
    printf(" after bcast: #%d has on position #%d, value = %d\n", my_rank, i, value[i]);

    }
    printf("*--------------------------------------*\n");
    
    // }
    MPI_Finalize();
    return 0;
    


}