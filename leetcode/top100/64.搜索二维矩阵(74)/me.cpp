#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
  bool searchMatrix(vector<vector<int>> &matrix, int target) {
    // 先搜索行，按照行首元素
    int left = 0, right = matrix.size()-1;
    int mid;
    while (left <= right) {
      mid = (left + right) / 2;
      if (matrix[mid][0] == target) {
        return true;
      } else {
        if (matrix[mid][0] < target) {
          left = mid + 1;
        } else {
          right = mid - 1;
        }
      }
    }
    int row = left - 1; //  指定行内搜索
    // 再搜索列，正常二分查找
    if (row < 0) {
      return false;
    }
    left = 0, right = matrix[0].size() - 1;
    while (left < right) {
      mid = (left + right) / 2;
      if (matrix[row][mid] == target) {
        return true;
      } else {
        if (matrix[row][mid] < target) {
          left = mid + 1;
        } else {
          right = mid - 1;
        }
      }
    }
    return false;
  }
};
int main() {
  Solution solution;
//   vector<vector<int>> matrix = {
//       {1, 3, 5, 7}, {10, 11, 16, 20}, {23, 30, 34, 60}};
//   int target = 3; // true


  vector<vector<int>> matrix = {{1,3}};
  int target = 3;
  bool result = solution.searchMatrix(matrix, target);
  cout << result << endl;
}
