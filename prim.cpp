#include <iostream>
#include <thread>
#include <vector>
#include <time.h>

using namespace std;

void print_matrix(vector<vector<int>> matrix)
{

  for (int i = 0; i < matrix.size(); ++i)
  {
    for (int j = 0; j < matrix[i].size(); ++j)
    {
      cout << matrix[i][j] << "\t";
    }
    cout << endl;
  }
}

vector<vector<int>> randomize_matrix(int n)
{
  srand((int)time(0));
  vector<vector<int>> matrix;
  for (int i = 0; i < n; ++i)
    matrix.push_back(vector<int>());

  for (int i = 0; i < n; ++i)
  {
    matrix[i].push_back(0);
    for (int j = i + 1; j < n; ++j)
    {
      int val = (rand() % 6) - 1;
      matrix[i].push_back(val);
      matrix[j].push_back(val);
    }
  }
  return matrix;
}

int main(int argc, char **argv)
{
  int n = atoi(argv[1]);
  if (n < 2)
  {
    cout << "Usage: ./a.out <n>";
  }

  print_matrix(randomize_matrix(n));
  return 0;
}