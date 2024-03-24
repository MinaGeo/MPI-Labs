// https://www.mpich.org/static/docs/v3.1.3/www3/MPI_Type_create_struct.html
// Create an MPI datatype from a general set of datatypes, displacements, and block sizes
//Copyrights to Eng.Ahmad Salama

#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

int main() {

    MPI_Init(NULL, NULL); 
    int world_size, my_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    int a = 0;
    double b = 0.0;
    char c[] = { '0', '0', '0', '\0' }; //sending an array of characters

    // MPI_AINT // datatype of MPI zay portBase keda fel RTOS
    MPI_Aint a_addr; 
    MPI_Aint b_addr;
    MPI_Aint c_addr;
    MPI_Get_address(&a, &a_addr); //this function get the address of a and put it in a_addr
    MPI_Get_address(&b, &b_addr); 
    MPI_Get_address(c, &c_addr);//no need to send it as address since it's an array

/*
int MPI Get address(
void∗ location p /∗ in ∗/,
MPI Aint∗ address p /∗ out ∗/);

*/
    int number_of_blocks = 3; //number of blocks = 3 (a,b,c)
    int elements_per_block[] = { 1, 1, 4 }; // elements per block, in our case 1 per block a,b but 4 for c as it's an array
    MPI_Aint displacement_of_each_block[] = { 0, b_addr - a_addr, c_addr - a_addr }; // el address of every element from the start 0 (address of a)
    MPI_Datatype data_type_of_each_block[] = { MPI_INT, MPI_DOUBLE, MPI_CHAR }; //the types of data in the blocks
    MPI_Datatype mina_struct; 

    MPI_Type_create_struct(
        number_of_blocks, // 3 elements a,b,c 
        elements_per_block, //{1,1,1}
        displacement_of_each_block, //{0, &b-&a, &c-&a}
        data_type_of_each_block, // {mpi_int, mpi_double, mpi_char}
        &mina_struct //esm el strucy bta3na
    );

    MPI_Type_commit(&mina_struct); //lazem a3ml commit to be able to use this struct
// int MPI Type commit(MPI Datatype∗ new mpi t p /∗ in/out ∗/);

    if (my_rank == 0) {
        a = 1;
        b = 2.0;
        c[0] = '1';
        c[1] = '2';
        c[2] = '3';
    }

    printf(" Before bcast, #%d has %d, %lf, %s\n", my_rank, a, b, c);
//lazem a3ml BCast so all process can access this struct
    MPI_Bcast(&a, 1, mina_struct, 0, MPI_COMM_WORLD);

    printf("  After bcast, #%d has %d, %lf, %s\n\n", my_rank, a, b, c);

    MPI_Type_free(&mina_struct); //b3ml delete lel struct so i don't allocate memory (b3ml free memory)
//    int MPI Type free(MPI Datatype∗ old mpi t p /∗ in/out ∗/);


    MPI_Aint testing_get_address;
    MPI_Get_address(&a,&testing_get_address);

    printf("address of a is %d", testing_get_address);
    MPI_Finalize();
    return 0;

}

