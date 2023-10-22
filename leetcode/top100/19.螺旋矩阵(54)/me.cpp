// 使用if判断行进或者转向
#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
  vector<int> spiralOrder(vector<vector<int>> &matrix) {
    int m = matrix.size(), n = matrix[0].size();
    vector<int> result;
    int direction = 0; // 行进方向，0右1下2左3上
    int i = 0, j = -1;  // 当前坐标
    int min_i = 1, max_i = m - 1, min_j = 0, max_j = n - 1; // 初始边界
    // int flag=0; // 连续转向的次数
    while (true) {
      if (direction == 0) {
        if ((j + 1) > max_j) {
          // 转向
          // flag ++;
          direction = 1;
          max_j--;
        } else {
          // 行进
          // flag = 0;
          j++;
          result.push_back(matrix[i][j]);
        }
      } else if (direction == 1) {
        if ((i + 1) > max_i) {
          // flag ++;
          direction = 2;
          max_i--;
        } else {
          // flag = 0;
          i++;
          result.push_back(matrix[i][j]);
        }
      } else if (direction == 2) {
        if ((j - 1) < min_j) {
          // flag ++;
          direction = 3;
          min_j++;
        } else {
          // flag = 0;
          j--;
          result.push_back(matrix[i][j]);
        }
      } else if (direction == 3) {
        if ((i - 1) < min_i) {
          // flag ++;
          direction = 0;
          min_i++;
        } else {
          // flag = 0
          i--;
          result.push_back(matrix[i][j]);
        }
      }
      // // 连续两次转向就要及时跳出循环
      // if (flag==2)
      //   break;
      // 获得元素数目足够时就跳出
      if(result.size()==m*n){
        break;
      }
    }
    return result;
  }
};

int main() {
  // vector<vector<int>> matrix = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
  // // [1,2,3,6,9,8,7,4,5]
  // vector<vector<int>> matrix = {{1,2,3,4},{5,6,7,8},{9,10,11,12}};
  // [1,2,3,4,8,12,11,10,9,5,6,7]
  vector<vector<int>> matrix = {{1,2,3,4,5},{6,7,8,9,10},{11,12,13,14,15},{16,17,18,19,20},{21,22,23,24,25}};
  // [1,2,3,4,5,10,15,20,25,24,23,22,21,16,11,6,7,8,9,14,19,18,17,12,13]
  Solution solution;
  vector<int> result = solution.spiralOrder(matrix);
  for (int i : result) {
    cout << i << " ";
  }
}