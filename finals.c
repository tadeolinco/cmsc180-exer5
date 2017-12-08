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

  int d = atoi(argv[1]);

  int hatian = d / processes;
  int sobra = d % processes; 

  long long *lengths = (long long *)malloc(sizeof(long long) * (d - 1));
  lengths[0] = 1;

  int multiplier = d - 2;
  for (i = 1; i < d - 1; ++i)
  {
    lengths[i] = multiplier * lengths[i - 1];
    multiplier--;
  }

  long double resistance = 0.0;
  for (i = 0; i < d - 1; ++i)
    resistance += lengths[i] * (1.0 / (i + 1));
  
  resistance = 1.0 / resistance;
  printf("%Lf\n", resistance);

  MPI_Finalize();
}