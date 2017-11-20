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
  int **matrix;
  randomize(&matrix, n, nprocs);
  int **product = NULL;
  if (myrank == 0)
  {
    product = (int **)malloc(sizeof(int *) * n);
    for (int i = 0; i < n; ++i)
    {
      product[i] = (int *)malloc(sizeof(int) * n);
    }
  }
  long long before = get_time();
  int vector[n];
  srand((int)time(0));
  for (int i = 0; i < n; ++i)
  {
    vector[i] = rand() % 10;
  }

    long long total_before = get_time();

  int cols = n / nprocs;
  int rows = n;
  before = get_time();
  for (int row = 0; row < rows; ++row)
  {
    for (int col = 0; col < cols; ++col)
    {
      matrix[row][col] *= vector[col];
    }
  }

  MPI_Gather(&(matrix[0][0]), n * n / nprocs, MPI_INT, product, n * n, MPI_INT, 0, MPI_COMM_WORLD);

  if (myrank == 0)
  {
    printf("Total time: %.3fs\n", (get_time() - total_before) / 1000.0);
  }

  // for (int i = 0; i < n; ++i)
  // {
  //   free(matrix[i]);
  // }
  // free(matrix);
  // if (myrank == 0)
  // {
  //   for (int i = 0; i < n; ++i)
  //   {
  //     free(product[i]);
  //   }
  //   free(product);
  // }
  MPI_Finalize();
  return 0;
}
