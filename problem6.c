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

  int i, j, k, a, b;
  int rank, processes;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &processes);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  srand((int)time(0));
  int n = processes / 2;

  int *mul_matrix = NULL;
  int *matrix = NULL;
  int *product_matrix = NULL;
  int sum = 0;
  if (rank == 0)
  {
    // initialize root matrix
    mul_matrix = (int *)malloc(sizeof(int) * n * n);
    matrix = (int *)malloc(sizeof(int) * n * n);
    product_matrix = (int *)malloc(sizeof(int) * n * n);
    for (i = 0; i < n * n; ++i)
    {
      matrix[i] = rand() % 10;
      mul_matrix[i] = rand() % 10;
    }
  }

  MPI_Scatter(matrix, 1, MPI_INT, &a, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Scatter(mul_matrix, 1, MPI_INT, &b, 1, MPI_INT, 0, MPI_COMM_WORLD);

  // initialize state
  if (rank == 2)
  {
    int temp = a;
    MPI_Recv(&a, 1, MPI_INT, 3, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Send(&temp, 1, MPI_INT, 3, 0, MPI_COMM_WORLD);
  }

  if (rank == 3)
  {
    MPI_Send(&a, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
    MPI_Recv(&a, 1, MPI_INT, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    MPI_Send(&b, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    MPI_Recv(&b, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }

  if (rank == 1)
  {
    int temp = b;
    MPI_Recv(&b, 1, MPI_INT, 3, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Send(&temp, 1, MPI_INT, 3, 0, MPI_COMM_WORLD);
  }

  for (i = 0; i < n; ++i)
  {
    sum += a * b;
    int a_dest = rank % n == 0 ? rank == 0 ? 1 : rank * n - 1 : rank - 1;
    int b_dest = rank + n >= n * n ? rank % n : rank + n;
    if (i != n - 1)
    {
      if (rank % 2 == 0)
      {
        MPI_Send(&a, 1, MPI_INT, a_dest, 0, MPI_COMM_WORLD);
        MPI_Recv(&a, 1, MPI_INT, a_dest, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      }
      else
      {
        int a_temp = a;
        MPI_Recv(&a, 1, MPI_INT, a_dest, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(&a_temp, 1, MPI_INT, a_dest, 0, MPI_COMM_WORLD);
      }

      if (rank < n)
      {

        MPI_Send(&b, 1, MPI_INT, b_dest, 0, MPI_COMM_WORLD);
        MPI_Recv(&b, 1, MPI_INT, b_dest, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      }
      else
      {
        int b_temp = b;
        MPI_Recv(&b, 1, MPI_INT, b_dest, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(&b_temp, 1, MPI_INT, b_dest, 0, MPI_COMM_WORLD);
      }
    }
  }
  MPI_Gather(&sum, 1, MPI_INT, product_matrix, 1, MPI_INT, 0, MPI_COMM_WORLD);

  if (rank == 0)
  {
    // output time for all to finish
    printf("Total time: %.3fs\n", (get_time() - then) / 1000.0);
  }

  // clean up
  if (rank == 0)
  {
    free(matrix);
    free(mul_matrix);
  }
}
