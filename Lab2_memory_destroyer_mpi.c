#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// To fix intellisense for VS Code, add "${MSMPI_INC}" to the include path in the workspace settings
// To compile, use: gcc {filename.c} -I $env:MSMPI_INC\ -L $env:MSMPI_LIB64\ -lmsmpi
// To execute, use: mpiexec -n M a.exe [this for M processes (replace M with number)]

#define N 7e8
//Law N = 7e8 and 1 process large time
//law N= 1e8 and more than 8 process (16 msln) hy3ml large time bardo
//Law N= 1e8 and 8 process better time -> 8 process btaccess el memory at the same time therefore will wait
//Law N= 1e8 and 4 process better time as well
// therefore there is an issue always in balancing between memory and process paralizing



int main(int argc, char const* argv[])
//argv is used to pass parameters when executing
{
//start first by initializing MPI 
MPI_Init(NULL,NULL);
//Initialize MPI size and rank
int my_rank, comm_size;
//MPI_COMM_WORLD contains the whole world(picture) of the processors
MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); // for rank
MPI_Comm_size(MPI_COMM_WORLD, &comm_size); //for size

/*now for this task we will have all the process that aren't
0 to allocate a place in the memory and initialize it, and will send an empty message
as a notification only to notify process 0 that it finished doing so.
Process 0 on the other hand will start counting using timer as soon as all processes start
and will stop the timer when it recieve this notification from all other processes 

*/
if(my_rank!=0) //indicates that this is any process but process 0
{
    int* arr = malloc(sizeof(int)*N); //here is allocation of memory which is N* 4 bytes (size of int)
    for(int i = 0; i<N;i++)
    {
        arr[i] = rand(); //here will initailze this memory with any random values
    }
    
    MPI_Send(NULL,0, 0, 0, 0, MPI_COMM_WORLD);
    //send the empty message as notification

}
else // you are process 0
{
    /*here we will use the concept of millis in arduino
    we will obtain the current time and put it in a variable known as start_time
    then we will do the operations after that we will store the new time in another variable known as end_time
     and subtract the end_time from start_time
    */

    // https://www.mpich.org/static/docs/v3.2/www3/MPI_Wtime.html
    double start_time = MPI_Wtime();
    for(int i = 1; i<comm_size; i++)
    {
        char a;
        MPI_Recv(NULL, 0,0, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
        //receieve the empty message from every process
    }
    double end_time = MPI_Wtime();
    printf("\nTime taken to initialize array: %fsec\n\n\n\n", end_time - start_time);

}

MPI_Finalize();

}
