#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
 
int main(int argc, char** argv) {
   MPI_Init(&argc, &argv);
 
   int rank, size;
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
 
   // Number of total random points
   int total_points = 1000000;
   // Points inside the quarter-circle
   int points_inside = 0;
 
   // Initialize random seed based on process rank
   srand(time(NULL) + rank);
 
   // Generate random points and count those inside the quarter-circle
   for (int i = 0; i < total_points; ++i) {
       double x = (double)rand() / RAND_MAX;
       double y = (double)rand() / RAND_MAX;
 
       if (x * x + y * y <= 1.0) {
           points_inside++;
       }}
 
   // Sum up points_inside across all processes using MPI_Reduce
   int global_points_inside;
   MPI_Reduce(&points_inside, &global_points_inside, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
 
   // Only the master process (rank 0) performs the final calculation and prints the result
   if (rank == 0) {
       double pi_estimate = 4.0 * global_points_inside / (double)(total_points * size);
       printf("Estimated value of pi: %lf\n", pi_estimate);
   }
 
   MPI_Finalize();
 
   return 0;}