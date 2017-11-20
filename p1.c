#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <sys/time.h>

long long get_time()
{
  struct timeval tv;

  gettimeofday(&tv, NULL);
  return (((long long)tv.tv_sec) * 1000) + (tv.tv_usec / 1000);
}

void randomize(int ***matrix, int n, int p)
{
  *matrix = (int **)malloc(sizeof(int *) * n);
  srand((int)time(0));
  for (int i = 0; i < n; ++i)
  {
    (*matrix)[i] = (int *)malloc(sizeof(int) * (n / p));
    for (int j = 0; j < (n / p); ++j)
    {
      int r = (rand() % 10);
      (*matrix)[i][j] = r;
    }
  }
}

int main(int argc, char **argv)
{

  int myrank, nprocs;
  int n = 10000;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

  if (myrank == 0)
  {
    printf("TEST for n = %d, p = %d\n", n, nprocs);
  }

  long long before = get_time();

  int **matrix;
  randomize(&matrix, n, nprocs);
  // printf("[%d]: Randomize Matrix: %.3fs\n", myrank, (get_time() - before) / 1000.0);

  long long total_before = get_time();

  int sum = 0;
  int cols = n / nprocs;
  int rows = n;
  before = get_time();
  for (int col = 0; col < cols; ++col)
  {
    for (int row = 0; row < rows; ++row)
    {
      sum += matrix[row][col];
    }
  }
  // printf("[%d]: Column sum: %.3fs\n", myrank, (get_time() - before) / 1000.0);

  int *sums = NULL;
  if (myrank == 0)
  {
    sums = (int *)malloc(sizeof(int) * nprocs);
  }

  MPI_Gather(&sum, 1, MPI_INT, sums, 1, MPI_INT, 0, MPI_COMM_WORLD);

  if (myrank == 0)
  {
    long total_sum = 0;
    for (int i = 0; i < nprocs; ++i)
    {
      total_sum += sums[i];
    }
    // printf("Total sum: %ld\n", total_sum);
    printf("Total time: %.3fs\n", (get_time() - total_before) / 1000.0);
    free(sums);
  }

  for (int i = 0; i < n; ++i)
  {
    free(matrix[i]);
  }
  free(matrix);
  MPI_Finalize();
  return 0;
}
