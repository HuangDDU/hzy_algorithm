// 按层遍历，向右向下可以，向左向上条件判断
#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
  vector<int> spiralOrder(vector<vector<int>> &matrix) {
    int m = matrix.size(), n = matrix[0].size();
    vector<int> result;
    int min_i = 0, max_i = m - 1, min_j = 0,
        max_j = n - 1; // 初始边界，都可以碰到
    while ((min_i <= max_i) && (min_j <= max_j)) {
      // 向右肯定可以
      for (int j = min_j; j <= max_j; j++) {
        result.push_back(matrix[min_i][j]);
      }
      for (int i = min_i + 1; i <= max_i; i++) {
        result.push_back(matrix[i][max_j]);
      }
      // 当内层至少有两层才可以
      if ((min_i < max_i) && (min_j < max_j)) {
        for (int j = max_j - 1; j > min_j; j--) {
          result.push_back(matrix[max_i][j]);
        }
        for (int i = max_i; i > min_i; i--) {
          result.push_back(matrix[i][min_j]);
        }
      }
      // 外层遍历完后
      min_i++, min_j++, max_i--, max_j--;
    }
    return result;
  }
};

int main() {
  // vector<vector<int>> matrix = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
  // // [1,2,3,6,9,8,7,4,5]
  // vector<vector<int>> matrix = {{1,2,3,4},{5,6,7,8},{9,10,11,12}};
  // [1,2,3,4,8,12,11,10,9,5,6,7]
  vector<vector<int>> matrix = {{1, 2, 3, 4, 5},
                                {6, 7, 8, 9, 10},
                                {11, 12, 13, 14, 15},
                                {16, 17, 18, 19, 20},
                                {21, 22, 23, 24, 25}};
  // [1,2,3,4,5,10,15,20,25,24,23,22,21,16,11,6,7,8,9,14,19,18,17,12,13]
  Solution solution;
  vector<int> result = solution.spiralOrder(matrix);
  for (int i : result) {
    cout << i << " ";
  }
}