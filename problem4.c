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

void print_matrix(int *matrix, int row, int col)
{
  int i, j;
  for (i = 0; i < row * col; ++i)
  {
    printf("%d ", matrix[i]);
    if (i % col == col - 1)
      printf("\n");
  }
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

  int *mul_matrix = (int *)malloc(sizeof(int) * n * n);
  if (rank == 0)
    for (i = 0; i < n * n; ++i)
      mul_matrix[i] = rand() % 10;
  // mul_matrix[i] = 1;
  MPI_Bcast(mul_matrix, n, MPI_INT, 0, MPI_COMM_WORLD);

  int *matrix = NULL;
  if (rank == 0)
  {
    // initialize root matrix
    matrix = (int *)malloc(sizeof(int) * n * n);
    for (i = 0; i < n * n; ++i)
      matrix[i] = rand() % 10;
    // matrix[i] = 1;
  }

  int *sub_matrix = (int *)malloc(sizeof(int) * n * n / processes);
  MPI_Scatter(matrix, n * n / processes, MPI_INT, sub_matrix, n * n / processes, MPI_INT, 0, MPI_COMM_WORLD);

  int *sub_product = (int *)malloc(sizeof(int) * n / processes);

  for (i = 0; i < n / processes; ++i)
  {
    sub_product[i] = 0;
    for (j = 0; j < n; ++j)
    {
      sub_product[i] += sub_matrix[i * n + j] * vector[j];
    }
  }

  int *product = NULL;
  if (rank == 0)
    product = (int *)malloc(sizeof(int) * n);

  MPI_Gather(sub_product, n / processes, MPI_INT, product, n / processes, MPI_INT, 0, MPI_COMM_WORLD);

  // output time for all to finish
  if (rank == 0)
  {
    printf("Total time: %.3fs\n", (get_time() - then) / 1000.0);
  }

  // clean up
  if (rank == 0)
  {
    free(matrix);
    free(product);
  }
  free(sub_matrix);
  free(sub_product);
  free(vector);
}

/*
for k in row * col
  for i in row
    for j in col
      matrix[k] += a[k / row * row + i] * b[k % col + j * col]
*/