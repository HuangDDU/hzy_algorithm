// 保存行进方向的而二维差值到数组上
#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
  vector<int> spiralOrder(vector<vector<int>> &matrix) {
    vector<vector<int>> directions = {
        {0, 1},
        {1, 0},
        {0, -1},
        {-1, 0}}; // 与之前一样下标0，1，2，3分别对应右、下、左、上。
    int rows = matrix.size(), columns = matrix[0].size();
    vector<vector<bool>> visited(rows, vector<bool>(columns)); 
    int total = rows * columns;
    vector<int> order(total);

    int row = 0, column = 0;
    int directionIndex = 0;
    for (int i = 0; i < total; i++) {
      order[i] = matrix[row][column];
      visited[row][column] = true;
      int nextRow = row + directions[directionIndex][0],
          nextColumn = column + directions[directionIndex][1]; // 尝试继续按照当前方向行进
      if (nextRow < 0 || nextRow >= rows || nextColumn < 0 ||
          nextColumn >= columns || visited[nextRow][nextColumn]) {
            // 超出边界或遇到访问的元素则转向
        directionIndex = (directionIndex + 1) % 4;
      }
      // 此时的方向可以继续行进
      row += directions[directionIndex][0];
      column += directions[directionIndex][1];
    }
    return order;
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