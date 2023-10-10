// 使用第一行、列标记是否需要设置
#include <iostream>
#include <set>
#include <vector>
using namespace std;
class Solution {
public:
    void setZeroes(vector<vector<int>>& matrix) {
        int m = matrix.size();
        int n = matrix[0].size();
        // 第一列是否有0原本用变量保存，此处用第一个元素保存
        int flag_col0 = false; 
        for (int i = 0; i < m; i++) {
            if (!matrix[i][0]) {
                flag_col0 = true;
            }
            for (int j = 1; j < n; j++) {
                if (!matrix[i][j]) {
                    matrix[i][0] = matrix[0][j] = 0;
                }
            }
        }
        // 这里的外循环从后往前更新 
        for (int i = m - 1; i >= 0; i--) {
            for (int j = 1; j < n; j++) {
                if (!matrix[i][0] || !matrix[0][j]) {
                    matrix[i][j] = 0;
                }
            }
            if (flag_col0) {
                matrix[i][0] = 0;
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