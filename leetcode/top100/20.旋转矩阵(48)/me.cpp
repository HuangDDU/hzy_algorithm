// 使用if判断行进或者转向
#include <iostream>
#include <vector>
using namespace std;

class Solution {
  // void exchange_(vector<vector<int>>& matrix, int)
public:
  void rotate(vector<vector<int>> &matrix) {
    int tmp = 0;
    for (int i = 0; i < (int)(matrix.size() / 2); i++) {
      int n = matrix.size() - 2 * i; // 方阵边长
      int start = i, end = i + n - 1;
      // 更新顶点， 交换三次
      tmp = matrix[start][start]; // 左上角->右上角
      matrix[start][start] = matrix[start][end];
      matrix[start][end] = tmp;
      tmp = matrix[end][start]; // 左下角->左上角
      matrix[end][start] = matrix[start][start];
      matrix[start][start] = tmp;
      // 右下角->左下角
      tmp = matrix[end][end];
      matrix[end][end] = matrix[end][start];
      matrix[end][start] = tmp;
      // 更新边, 交换三次
      // 左边->上边
      for (int i = 1; i < n - 1; i++) {
        tmp = matrix[start + i][start];
        matrix[start + i][start] = matrix[start][end - i];
        matrix[start][end - i] = tmp;
      }
      // 下边->左边
      for (int i = 1; i < n - 1; i++) {
        tmp = matrix[end][start + i];
        matrix[end][start + i] = matrix[start + i][start];
        matrix[start + i][start] = tmp;
      }
      // 右边->下边
      for (int i = 1; i < n - 1; i++) {
        tmp = matrix[start + i][end];
        matrix[start + i][end] = matrix[end][end - i];
        matrix[end][end - i] = tmp;
      }
    }
  }
};

int main() {
  //   vector<vector<int>> matrix = {
  //       {1, 2, 3}, {4, 5, 6}, {7, 8, 9}};  // [7,4,1],[8,5,2],[9,6,3]

  vector<vector<int>> matrix = {
      {5, 1, 9, 11}, {2, 4, 8, 10}, {13, 3, 6, 7}, {15, 14, 12, 16}};
  // [[15,13,2,5],[14,3,4,1],[12,6,8,9],[16,7,10,11]]

  Solution solution;
  solution.rotate(matrix);
  for (vector<int> item : matrix) {
    for (int i : item) {
      cout << i << " ";
    }
    cout << endl;
  }
}