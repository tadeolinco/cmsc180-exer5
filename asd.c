#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
  srand((int)time(0));
  int n = atoi(argv[1]);

  // initialize matrix to multiply with
  int *mul_matrix = (int *)malloc(sizeof(int) * n * n);
  int *product = (int *)malloc(sizeof(int) * n * n);
  for (i = 0; i < n * n; ++i)
    mul_matrix[i] = rand() % 10;

  // initialize matrix
  int *matrix = NULL;
  // initialize root matrix
  matrix = (int *)malloc(sizeof(int) * n * n);
  for (i = 0; i < n * n; ++i)
    matrix[i] = rand() % 10;

  // compute for matrix multiplication
  for (i = 0; i < n; ++i)
  {
    for (j = 0; j < n; ++j)
    {
      int sum = 0;
      for (k = 0; k < n; ++k)
      {
        sum += matrix[i * n + k] * mul_matrix[k * n + j];
      }
      product[i * n + j] = sum;
    }
  }

  // output time for all to finish
  printf("Total time: %.3fs\n", (get_time() - then) / 1000.0);

  // clean up
  free(matrix);
  free(mul_matrix);
}
