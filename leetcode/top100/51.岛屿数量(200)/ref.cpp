#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
  int numIslands(vector<vector<char>> &grid) {
    // 找网格地图中的连通分量个数
    int count = 0;
    int m = grid.size(), n = grid[0].size();
    for (int i = 0; i < m; i++) {
      for (int j = 0; j < n; j++) {
        if (grid[i][j] == '1') {
          // 找到未遍历的岛屿
          count++;
          dfs(grid, i, j);
        }
      }
    }
    return count;
  }
  void dfs(vector<vector<char>> &grid, int i, int j) {
    if ((i < 0) || (j < 0) || (i >= grid.size()) || (j >= grid[0].size()) || (grid[i][j] != '1')) {
      return;
    }
    grid[i][j] = '2';
    // 递归计算
    dfs(grid, i - 1, j); // 上
    dfs(grid, i, j - 1); // 左
    dfs(grid, i + 1, j); // 下
    dfs(grid, i, j + 1); // 右
  }
};

int main() {
  Solution solution;
  //   vector<vector<char>> grid = {
  //       {'1', '1', '1', '1', '0'},
  //       {'1', '1', '0', '1', '0'},
  //       {'1', '1', '0', '0', '0'},
  //       {'0', '0', '0', '0', '0'},
  //   }; // 1
  vector<vector<char>> grid = {
      {'1', '1', '0', '0', '0'},
      {'1', '1', '0', '0', '0'},
      {'0', '0', '1', '0', '0'},
      {'0', '0', '0', '1', '1'},
  }; // 3
  int result = solution.numIslands(grid);
  cout << result << endl;
}
