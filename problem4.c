#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <sys/time.h>
#include <unistd.h>

long long get_time()
{
  struct timeval tv;

  gettimeofday(&tv, NULL);
  return (((long long)tv.tv_sec) * 1000) + (tv.tv_usec / 1000);
}

int main(int argc, char **argv)
{
  long long then = get_time();

  int i, j, k;
  int rank, processes;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &processes);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  srand((int)time(0));
  int n = atoi(argv[1]);

  // initialize matrix to multiply with
  int *mul_matrix = (int *)malloc(sizeof(int) * n * n);
  if (rank == 0)
    for (i = 0; i < n * n; ++i)
      mul_matrix[i] = rand() % 10;

  // broadcast so that other processes also have it
  MPI_Bcast(mul_matrix, n * n, MPI_INT, 0, MPI_COMM_WORLD);

  // initialize matrix
  int *matrix = NULL;
  if (rank == 0)
  {
    // initialize root matrix
    matrix = (int *)malloc(sizeof(int) * n * n);
    for (i = 0; i < n * n; ++i)
      matrix[i] = rand() % 10;
  }

  // holder for sub parts of the matrix
  int *sub_matrix = (int *)malloc(sizeof(int) * n * n / processes);
  MPI_Scatter(matrix, n * n / processes, MPI_INT, sub_matrix, n * n / processes, MPI_INT, 0, MPI_COMM_WORLD);

  // holder for a part of the product
  int *sub_product = (int *)malloc(sizeof(int) * n * n / processes);

  // compute for matrix multiplication
  for (i = 0; i < n / processes; ++i)
  {
    for (j = 0; j < n; ++j)
    {
      int sum = 0;
      for (k = 0; k < n; ++k)
      {
        sum += sub_matrix[i * n + k] * mul_matrix[k * n + j];
      }
      sub_product[i * n + j] = sum;
    }
  }

  int *product = NULL;
  if (rank == 0)
    product = (int *)malloc(sizeof(int) * n * n);

  // gather sub products into product
  MPI_Gather(sub_product, n * n / processes, MPI_INT, product, n * n / processes, MPI_INT, 0, MPI_COMM_WORLD);

  // output time for all to finish
  printf("Total time: %.3fs\n", (get_time() - then) / 1000.0);

  // clean up
  if (rank == 0)
  {
    free(matrix);
    free(product);
  }
  free(sub_matrix);
  free(sub_product);
  free(mul_matrix);
}
