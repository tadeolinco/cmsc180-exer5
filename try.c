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

  // n = 6
  // total = 4
  // int total = n - 2;
  // int multiplier = total;
  // int prods[total];
  // for (i = 0; i < total; ++i)
  //   prods[i] = 1;

  // int sector = (total - 1) / processes;
  // int offset = (total - 1) % processes;
  // for (k = 0; k < processes; ++k)
  // {
  //   if (k < rank)
  //     sector++;

  // }

  // if (rank == 0)
  // {
  //   for (i = 0; i < total; ++i)
  //   {
  //     printf("%d ", prods[i]);
  //   }
  //   printf("\n");
  // }

  long long total = n - 2;
  long long prods[total];
  for (i = 0; i < total; ++i)
    prods[i] = total;

  long long offset = 1;
  for (i = total - 1; i > 1; --i)
  {
    for (j = offset; j < total; ++j)
    {
      prods[j] *= i;
    }
    offset++;
  }
  if (rank == 0)
  {
    for (i = 0; i < total; ++i)
    {
      printf("%d ", prods[i]);
    }
    printf("\n");
  }
  MPI_Finalize();
}