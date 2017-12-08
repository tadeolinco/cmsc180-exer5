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
  {
    vector<int> row;
    for (int j = 0; j < n; ++j)
    {
      int val = rand() % 6;
      row.push_back(val);
    }
    matrix.push_back(row);
  }
  return matrix;
}

void multiply_matrix()
{
  cout << "Hello";
}

int main(int argc, char **argv)
{
  int n = atoi(argv[1]);
  int p = atoi(argv[2]);
  if (n < 3)
  {
    cout << "Usage: ./a.out <n> <p>";
  }
  vector<vector<int>> matrix_a = randomize_matrix(n);
  vector<vector<int>> matrix_b = randomize_matrix(n);
  vector<vector<vector<int>>> sub_matrix;
  vector<thread> threads(p);

  int sector = n / p;
  int offset = n % p;

  for (int i = 0; i < p; ++p)
  {
    threads.at(i) = thread{multiply_matrix};
    threads.at(i).join();
  }

  return 0;
}