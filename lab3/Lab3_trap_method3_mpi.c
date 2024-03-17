#include <string.h>
#include <stdio.h>
#include <mpi.h>
#include <math.h>

// To fix intellisense for VS Code, add "${MSMPI_INC}" to the include path in the workspace settings
// To compile, use: gcc 1.1-hello-mpi.c -I $env:MSMPI_INC\ -L $env:MSMPI_LIB64\ -lmsmpi
// To execute, use: mpiexec -n M a.exe [this for M processes (replace M with number)]


double f(double x) { return 3 * pow(x, 2.0); }
double fdash(double a, double b) { return (pow(b, 3.0) - pow(a, 3.0)); }
double Trap(double a, double b, double n, double h, int my_rank)
{
    // printf("a: %f, b: %f, n: %f, h: %f, my_rank: %f", a, b, n, h, my_rank);
    double approx = 0.0;
    approx += f(a) / 2 + f(b) / 2;
    for (int i = 1; i < n; i++)
        approx += f(a + i * h);
    return h * approx;
}

int main()
{

    // Initialize the MPI execution environment
    MPI_Init(NULL, NULL);
    // Get how many processes are running in a given communicator; for example the default "MPI_COMM_WORLD"
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    // And get the rank of the calling process within that communicator
    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // double a  = 0, b = 200, n = 2048; // this time the a, b, and n are taken by the processers
    // double h = (b - a) / n;
    double a,b,n,h;

    if (my_rank == 0) {
        printf("Please enter a: ");
        fflush(stdout); // Necessary for printf to print. Why do you think it is needed?
        scanf("%lf", &a);
        printf("Please enter b: ");
        fflush(stdout); // Necessary for printf to print. Why do you think it is needed?
        scanf("%lf", &b);
        printf("Please enter n: ");
        fflush(stdout); // Necessary for printf to print. Why do you think it is needed?
        scanf("%lf", &n);
    }

    MPI_Bcast(&a, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&b, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&n, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    h = (b - a) / n; // Or would it be better to Bcast it as well?

    double local_n = n / world_size;
    double local_a = a + local_n * my_rank * h;
    double local_b = local_a + h * local_n;
    double local_h = (local_b - local_a) / local_n;

    double local_area = Trap(local_a, local_b, local_n, local_h, my_rank);
    //printf("Debug: %f\n", local_area);

    // // Finally, hello MPI!
    // if (my_rank != 0)
    // {
    //     // If I'm not process #0, I will write my greeting into the character array "greeting" using sprintf
    //     // Then I will send my greeting to process #0 using MPI_Send as follows:
    //     MPI_Send(
    //         &local_area,   // A pointer that points at the beginning of the data that I want to send
    //         1,             // The amount of data that I want to send (+ 1 because strlen doesn't count null character)
    //         MPI_DOUBLE,    // The type of each element in the data that I want to send
    //         0,             // The target process, in this case I want to send my greeting to process #0
    //         0,             // Tag - we'll know more about it later
    //         MPI_COMM_WORLD // Communicator - for now we'll use the default
    //     );
    // }
    // else
    // {
        double total = 0;
        // total = total + local_area;
        MPI_Reduce(&local_area, &total, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    if (my_rank == 0) {
        printf("Approximate cumulative integral via the trapezoidal method (512 steps): %f\n", total);
        printf("                                                   Analytical integral: %f\n", fdash(a, b));
    }
      
    //     for (int i = 1; i < world_size; i++)
    //     {
    //         MPI_Recv(
    //             &local_area,      // A pointer that points at the beginning of the memory where I want to receive data
    //             1,                // Maximum number of elements I can receive
    //             MPI_DOUBLE,       // The type of each element in the data that I want to receive
    //             i,                // The sender process, in this case I am receiving from each process, per iteration
    //             0,                // Tag - we'll know more about it later
    //             MPI_COMM_WORLD,   // Communicator - for now we'll use the default
    //             MPI_STATUS_IGNORE // Status object, we'll know more about it later also
    //         );
    //         total+= local_area;

    //     }

        // printf("Approximate cumulative integral via the trapezoidal method ( %f steps ): %f\n", n,total);
        // printf("                                                   Analytical integral: %f\n", fdash(a, b));
    // }

    // Terminates MPI execution environment
    MPI_Finalize();
}
