#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

#define ROWS 9
#define COLS 5

// This is just to visualize the problem to you my dear students, you may ignore it
void print_matrix_and_vectors(int*, int*, int*);

int main() {

    MPI_Init(NULL, NULL);
    int world_size, my_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // We want to do vector_out = matrix * vector_in
    int* vector_in = (int*) malloc(COLS * sizeof(int)); // Shared, need to malloc for all
    // XXX: We can't use 2D arrays with MPI! MPI functions expect contiguous data
    // int** matrix;
    int* matrix; // Unique to rank #0 process, will only allocate memory for it
    int* vector_out; // Unique to rank #0 process, will only allocate memory for it

    // Rank #0 shall randomly initialize vectors 1 and 2 and print them
    if (my_rank == 0) {
        matrix = (int*) malloc(ROWS * COLS * sizeof(int));
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                matrix[COLS * i + j] = rand() % 5; // randomly initialized every cell in Rows*cols matrix
                // keda 5alet el matrix dah 1D 3lshan a7el el moshkela (that's why b3ml cols*i+j) 
            }
          
        }

        for (int i = 0; i < COLS; i++)
        { vector_in[i] = rand() % 4; //randomly inilialized vector_in
        }

        vector_out = (int*) calloc(ROWS, sizeof(int)); //initilaize 0

    }

    // Since they all use vector_in, let's broadcast it!
    MPI_Bcast(vector_in, COLS, MPI_INT, 0, MPI_COMM_WORLD); 

    // And now let's scatter the matrix row by row!
    // XXX: Remember: You can't use 2-dimensional arrays here
    int local_rows_num = ROWS / world_size;
    int* local_rows = (int*) malloc(local_rows_num * COLS * sizeof(int)); //h2sm masln kaza element fel row lel process hthst8l 3aleeh 
    MPI_Scatter(
        matrix, local_rows_num * COLS, MPI_INT,
        local_rows, local_rows_num * COLS, MPI_INT,
        0, MPI_COMM_WORLD
    ); //we will scatter the matrix 3lshan nb3t a row to every process

    // XXX: Now let's get the sumsss of products of each row by vector_in
    int* sums = calloc(local_rows_num, sizeof(int));
    for (int i = 0; i < local_rows_num; i++) {
        for (int j = 0; j < COLS; j++) {
            sums[i] += local_rows[COLS * i + j] * vector_in[j]; //hn3gm3 sum of products which is every row * vector 
        }
    }

    // XXX: And now we gather those sumssssss into vector_out
    MPI_Gather(
        sums, local_rows_num, MPI_INT,
        vector_out, local_rows_num, MPI_INT,
        0, MPI_COMM_WORLD
    ); //h3ml gather all sums of products from every process to vector_out (row in a row)
// if (my_rank == 0) {
//     int remaining_items = ROWS * COLS - local_rows_num * world_size;
//   //  int offset = local_rows_num * world_size; // Determine the starting index for remaining items
//     int j;
//     // Process remaining items
//     for (int i = 0; i < remaining_items; i++) {
//          j = ROWS * COLS - i - 1; // 
//         sums[j] = vector_out[j] * vector_in[j];
//     }
// }


    if (my_rank == 0) print_matrix_and_vectors(matrix, vector_in, vector_out);

    MPI_Finalize();
    return 0;

}

/*
THE ISSUE OF MPI IS THAT IT DOESN'T ALLOW THE REPRESENTATION OF DATA IN MPI 3lshan by3bt el data as a pointers
therefore hn3ml represent lel array dah as 1D array




*/

//function btm3ml print
void print_matrix_and_vectors(int* mat, int* vec_in, int* vec_out) {
    printf("\n");
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%d ", mat[COLS * i + j]);
        }
        if (i == COLS / 2) {
            printf(" *  %d  =  %d", vec_in[i], vec_out[i]);
        } else if (i < COLS) {
            printf("    %d     %d", vec_in[i], vec_out[i]);
        } else {
            printf("          %d", vec_out[i]);
        }
        printf("\n");
    }
    printf("\n");
}
