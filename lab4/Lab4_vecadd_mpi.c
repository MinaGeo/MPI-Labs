// Imagine you're once again in a situation like the trapezoidal problem
// But this time, process #0 has all of the data from the start
// To recalculate data at each other process would be a waste
// Why don't we instead "scatter" this data onto them?
// Then "gather" the output from them back to process #0.

// The example for this is the vector addition problem (adding two vectors to get a 3rd one)
//To fix intellisense for VS Code, add "${MSMPI_INC}" to the include path in the workspace settings
//To compile, use: gcc 1.1-hello-mpi.c -I $env:MSMPI_INC\ -L $env:MSMPI_LIB64\ -lmsmpi
//To execute, use: mpiexec -n M a.exe [this for M processes (replace M with number)]

#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

#define VECTOR_SIZE 14

int main() {

    MPI_Init(NULL, NULL);
    int world_size, my_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // We want to do vector3 = vector1 + vector2;
    int* vector1;
    int* vector2;
    int* vector3;

    // Rank #0 shall randomly initialize vectors 1 and 2 and print them
    if (my_rank == 0) {
        vector1 = (int*) malloc(VECTOR_SIZE * sizeof(int)); //b3ml allocation lel vector1 
        vector2 = (int*) malloc(VECTOR_SIZE * sizeof(int)); //b3ml allocation lel vector2
        vector3 = (int*) calloc(VECTOR_SIZE, sizeof(int)); // I use calloc to init to 0 //b3ml allocation lel vector3 bas with 0 intialized
        for (int i = 0; i < VECTOR_SIZE; i++)
        {vector1[i] = 10 + rand() % 100;} //bfill vector1 with random numbers
        for (int i = 0; i < VECTOR_SIZE; i++)
        {vector2[i] = 10 + rand() % 100;} //bfill vector2 with random numbers
        printf("1: [\t");
        for (int i = 0; i < VECTOR_SIZE; i++)
        { printf("%d\t", vector1[i]); }
        printf("]\n\n");
        printf("2: [\t");
        for (int i = 0; i < VECTOR_SIZE; i++)
        { printf("%d\t", vector2[i]); }
        printf("]\n\n");
    }

    // Now let's scatter these vectors!
    int local_size = VECTOR_SIZE / world_size; //b2sm el vector size 3la number of processes
    int* local_vector1 = (int*) malloc(local_size * sizeof(int)); //b3ml intialize lel mkan ely hst2bl feeh
    int* local_vector2 = (int*) malloc(local_size * sizeof(int));//b3ml intialize lel mkan ely hst2bl feeh
    int* local_vector3 = (int*) malloc(local_size * sizeof(int));//b3ml intialize lel mkan ely hst2bl feeh

    MPI_Scatter(
        vector1, local_size, MPI_INT,
        local_vector1, local_size, MPI_INT,
        0, MPI_COMM_WORLD
    ); //b3ml scatter men vector1 lel local_vector1
    MPI_Scatter(
        vector2, local_size, MPI_INT,
        local_vector2, local_size, MPI_INT,
        0, MPI_COMM_WORLD
    ); //b3ml scatter men vector2 lel local_vector2

    // Sum each element pair locally into the local output vector
    for (int i = 0; i < local_size; i++) {
        local_vector3[i] = local_vector1[i] + local_vector2[i];
    }

    // And now we gather!
    MPI_Gather(
        local_vector3, local_size, MPI_INT,
        vector3, local_size, MPI_INT,
        0, MPI_COMM_WORLD
    );

    // You need to check if the vector was scattered completely or not!
    // If not, have rank #0 calculate the unscattered part
    if (my_rank == 0) {
        int remaining_items = VECTOR_SIZE - local_size * world_size; // 3dd el elements el m7lshomsh add 14-12 = 2
        int j;
        for (int i = 0; i < remaining_items; i++) {
            j = VECTOR_SIZE - i - 1; // 
            vector3[j] = vector1[j] + vector2[j];
        }
    } //3mlna el goz2 dah 3lshan msln law 3ndy 14 elements, then dividing it on 4 process is not possible 14/4 = 3.[] 
    // therefore this place make sure that any numbers that weren't divided on any process will be added.

    // Finally, process #0 shall print the result. 
    if (my_rank == 0) {
        printf("3: [\t");
        for (int i = 0; i < VECTOR_SIZE; i++) printf("%d\t", vector3[i]);
        printf("]\n\n");
    }

    MPI_Finalize();
    return 0;

}