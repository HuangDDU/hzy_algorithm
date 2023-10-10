// 保存需要设置为0的行列号后，然后设置
#include <iostream>
#include <set>
#include <vector>
using namespace std;

class Solution {
public:
  void setZeroes(vector<vector<int>> &matrix) {
    int m = matrix.size(), n = matrix[0].size();
    // 保存需要设置为0的行列号
    set<int> row_set, col_set;
    for (int i = 0; i < m; i++) {
      for (int j = 0; j < n; j++) {
        if (matrix[i][j] == 0) {
          row_set.insert(i);
          col_set.insert(j);
        }
      }
    }
    // 设置对应的行列号
    for (int row : row_set) {
      for (int i = 0; i < n; i++) {
        matrix[row][i] = 0;
      }
    }
    for (int col : col_set) {
      for (int i = 0; i < m; i++) {
        matrix[i][col] = 0;
      }
    }
  }
};

int main() {
  vector<vector<int>> matrix = {{1, 1, 1}, {1, 0, 1}, {1, 1, 1}};
  // [[1,0,1],[0,0,0],[1,0,1]]
  Solution solution;
  solution.setZeroes(matrix);
  for (vector<int> item : matrix) {
    for (int i : item) {
      cout << i << " ";
    }
    cout << endl;
  }
}