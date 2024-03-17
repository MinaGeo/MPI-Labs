#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// To fix intellisense for VS Code, add "${MSMPI_INC}" to the include path in the workspace settings
// To compile, use: gcc {filename.c} -I $env:MSMPI_INC\ -L $env:MSMPI_LIB64\ -lmsmpi
// To execute, use: mpiexec -n M a.exe [this for M processes (replace M with number)]

#define MAX_ARRAY_SIZE 1000

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
//Create an array of MAX size
char names[MAX_ARRAY_SIZE];


/*process 0 is the one that creates the message
which will be accessed by the rest of the processes
*/

if(my_rank==0) //indicates that this
{
    strcpy(names, "I stay with the bag just like shardai");
}
else //you are any other process but process 0
{
MPI_Recv(names,MAX_ARRAY_SIZE,MPI_CHAR, my_rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//you will recieve the message from the process before you
//therefore it's my_rank - 1
}

//now to the main part which is telephone 5arban game where we randomly
//will change a character in the string above
srand(my_rank); //srand is random with seed, and since we want unique seeds
//then we will put my_rank as the seed parameter as it's unique
names[rand()%strlen(names)] = 'a'+rand()%26; //here will chose a random 
//character in the array and change it with another character

MPI_Send(names,strlen(names) +1,MPI_CHAR, (my_rank+1)%comm_size,0, MPI_COMM_WORLD);
/*in here we will send the message to the next process thereofre 
we wrote (my_rank+1), but why %comm_size? because when we reach process N-1
it should go to process 0, not process N, therefore we used %

*/

//After changing the names array we want process 0 again to receieve the message after
//it has been changed and print it

if(my_rank==0)
{
    //we will recieve from comm_size -1 which indicates last process
    MPI_Recv(names, strlen(names)+ 1, MPI_CHAR, comm_size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("%s", names);
}







//At the end you always must finialize the MPI enviroment
MPI_Finalize();
return 0;

}