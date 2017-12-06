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

int main(int argc, char **argv)
{
  int i, j, k;
  int rank, processes;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &processes);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int n = atoi(argv[1]);
  int total_sum = 0;
  int sub_sum = 0;
  int **matrix = NULL;

  int **sub_matrix = (int **)malloc(sizeof(int *) * n);
  for (i = 0; i < n; ++i)
  {
    sub_matrix[i] = (int *)malloc(sizeof(int) * n / processes);
  }

  long long then = get_time();

  if (rank == 0)
  {
    srand((int)time(0));
    matrix = (int **)malloc(sizeof(int *) * n);
    for (i = 0; i < n; ++i)
    {
      matrix[i] = (int *)malloc(sizeof(int) * n);
      for (j = 0; j < n; ++j)
      {
        matrix[i][j] = rand() % 10;
      }
    }

    for (k = 0; k < processes; ++k)
    {
      for (i = 0; i < n; ++i)
      {
        int start_col = k * n / processes;
        for (j = start_col; j < start_col + n / processes; ++j)
        {
          if (k == 0)
            sub_matrix[i][j] = matrix[i][j];
          else
            MPI_Send(&matrix[i][j], 1, MPI_INT, k, 0, MPI_COMM_WORLD);
        }
      }
    }
  }
  else
  {
    for (i = 0; i < n; ++i)
    {
      for (j = 0; j < n / processes; ++j)
      {
        MPI_Recv(&sub_matrix[i][j], 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      }
    }
  }

  for (i = 0; i < n; ++i)
    for (j = 0; j < n / processes; ++j)
      sub_sum += sub_matrix[i][j];

  if (rank == 0)
  {
    total_sum += sub_sum;
    for (k = 1; k < processes; ++k)
    {
      MPI_Recv(&sub_sum, 1, MPI_INT, k, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      total_sum += sub_sum;
    }
    printf("Total sum %d\n", total_sum);
  }
  else
  {
    MPI_Send(&sub_sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }

  printf("Total time: %.3fs\n", (get_time() - then) / 1000.0);
  if (rank == 0)
  {
    for (i = 0; i < n; ++i)
    {
      free(matrix[i]);
    }
    free(matrix);
  }
  for (i = 0; i < n / processes; ++i)
  {
    free(sub_matrix[i]);
  }
  free(sub_matrix);
}