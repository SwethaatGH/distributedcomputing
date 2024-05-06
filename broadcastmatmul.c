#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"
#include <time.h>
#include <sys/time.h>
 
// Number of rows and columns in a matrix
#define N 4
 
MPI_Status status;
 
// Matrix holders are created
double matrix_a[N][N], matrix_b[N][N], matrix_c[N][N];
 
int main(int argc, char **argv) {
   int processCount, processId, slaveTaskCount, source, rows, offset;
   struct timeval start, stop;
 
   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &processId);
   MPI_Comm_size(MPI_COMM_WORLD, &processCount);
 
   slaveTaskCount = processCount - 1;
 
   if (processId == 0) {
       srand(time(NULL));
 
       for (int i = 0; i < N; i++) {
           for (int j = 0; j < N; j++) {
               matrix_a[i][j] = rand() % 10;
               matrix_b[i][j] = rand() % 10;
           }
       }
 
       printf("\n\t\tMatrix - Matrix Multiplication using MPI\n");
       printf("\nMatrix A\n\n");
 
       for (int i = 0; i < N; i++) {
           for (int j = 0; j < N; j++) {
               printf("%.0f\t", matrix_a[i][j]);
           }
           printf("\n");
       }
 
       printf("\nMatrix B\n\n");
 
       for (int i = 0; i < N; i++) {
           for (int j = 0; j < N; j++) {
               printf("%.0f\t", matrix_b[i][j]);
           }
           printf("\n");
       }
 
       rows = N / slaveTaskCount;
       offset = 0;
 
       // Broadcast matrix_a and matrix_b to all processes
       MPI_Bcast(matrix_a, N * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
       MPI_Bcast(matrix_b, N * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
 
       for (int dest = 1; dest <= slaveTaskCount; dest++) {
           MPI_Send(&offset, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
           MPI_Send(&rows, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
 
           offset = offset + rows;
       }
 
       for (int i = 1; i <= slaveTaskCount; i++) {
           source = i;
 
           MPI_Recv(&offset, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
           MPI_Recv(&rows, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
           MPI_Recv(&matrix_c[offset][0], rows * N, MPI_DOUBLE, source, 2, MPI_COMM_WORLD, &status);
       }
 
       printf("\nResult Matrix C = Matrix A * Matrix B:\n\n");
 
       for (int i = 0; i < N; i++) {
           for (int j = 0; j < N; j++)
               printf("%.0f\t", matrix_c[i][j]);
           printf("\n");
       }
 
       printf("\n");
   }
 
   if (processId > 0) {
       // Broadcast matrix_a and matrix_b to all slave processes
       MPI_Bcast(matrix_a, N * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
       MPI_Bcast(matrix_b, N * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
 
       MPI_Recv(&offset, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
       MPI_Recv(&rows, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
 
       for (int k = 0; k < N; k++) {
           for (int i = 0; i < rows; i++) {
               matrix_c[i][k] = 0.0;
               for (int j = 0; j < N; j++)
                   matrix_c[i][k] += matrix_a[i][j] * matrix_b[j][k];
           }
       }
 
       MPI_Send(&offset, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
       MPI_Send(&rows, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
       MPI_Send(&matrix_c, rows * N, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
   }
 
   MPI_Finalize();
 
   return 0;
}