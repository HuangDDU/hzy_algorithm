#include <iostream>
#include <vector>
using namespace std;
class Solution {
public:
  int orangesRotting(vector<vector<int>> &grid) {
    int result = 0;
    int m = grid.size(), n = grid[0].size();
    while (true) {
      bool flag = false;
      for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
          if (grid[i][j] == 2) {
            // 临近点本轮dfs会影响当前结点的dfs
            bool up = dfs(grid, i - 1, j);
            bool left = dfs(grid, i, j - 1);
            bool down = dfs(grid, i + 1, j);
            bool right = dfs(grid, i, j + 1);
            if (up || left || down || right) {
              flag = true;
            }
          }
        }
      }
      if (!flag) {
        break;
      }
      result++;
    }
    for (int i = 0; i < m; i++) {
      for (int j = 0; j < n; j++) {
        if (grid[i][j] == 1) {
          return -1;
        }
      }
    }
    return result;
  }

  bool dfs(vector<vector<int>> &grid, int i, int j) {
    if ((i < 0) || (j < 0) || (i >= grid.size()) || (j >= grid[0].size()) ||
        (grid[i][j] == 0) || (grid[i][j] == 2)) {
      return false;
    }
    grid[i][j] = 2;
    return true;
  }
};
int main() {
  Solution solution;

  vector<vector<int>> grid = {{2, 1, 1}, {1, 1, 0}, {0, 1, 1}}; // 4
  int result = solution.orangesRotting(grid);
  cout << result << endl;
}
