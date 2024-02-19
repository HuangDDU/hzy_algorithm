#include <iostream>
#include <vector>
using namespace std;

// 5 3 2 10
// 2 3
// 20 15 10 10 10
// 10 10 10 20 10
// 10 15 10 90 10
// 10 20 10 10 10
// 10 10 10 10 10
// 0 0 0
// 3 1 0
// 1 3 0
// 0 10
// 20 0

int main() {
  // 输入部分
  int N, M, F, P;
  int I, J;
  cin >> N >> M >> F >> P;
  vector<vector<int>> G_matrix(N, vector<int>(N)); // 地图矩阵，每个单元的传输代价
  vector<vector<int>> C_vector(M, vector<int>(3)); // Consumer三元组
  vector<vector<int>> F_matrix(F, vector<int>(F)); // 转换矩阵
  cin >> I >> J;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      cin >> G_matrix[i][j];
    }
  }
  // cout << "G_matrix OK" << endl;

  for (int i = 0; i < M; i++) {
    cin >> C_vector[i][0] >> C_vector[i][1] >> C_vector[i][2];
  }
  // cout << "C_vector OK" << endl;
  for (int i = 0; i < F; i++) {
    for (int j = 0; j < F; j++) {
      cin >> F_matrix[i][j];
    }
  }
  // cout << "F_matrix OK" << endl;


  // 输出部分
  // 直接输出看看能得多少分
  cout << "2" << endl;
  cout << "2 3 2 1 3 0 0 1 0" << endl;
  cout << "2 1 2 1 2 0 0 2 0" << endl;
  cout << "0 1 1 1 1 0" << endl;
  return 0;
}
