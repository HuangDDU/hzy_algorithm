#include <iostream>
#include <queue>
#include <vector>

using namespace std;
class Solution {
public:
  int orangesRotting(vector<vector<int>> &grid) {
    int result = -1; // 污染轮次
    // 找到初始腐烂橘子
    int m = grid.size(), n = grid[0].size();
    int fresh_count = 0;
    queue<vector<int>> q;
    for (int i = 0; i < m; i++) {
      for (int j = 0; j < n; j++) {
        if (grid[i][j] == 2) {
          // 腐烂橘子
          q.push({i, j});
        } else if (grid[i][j] == 1) {
          // 新鲜橘子
          fresh_count++;
        }
      }
    }
    // bfs
    while (!q.empty()) {
      int tmp_n = q.size();
      for (int k = 0; k < tmp_n; k++) {
        vector<int> orange_pos = q.front();
        q.pop();
        int i = orange_pos[0], j = orange_pos[1];
        vector<vector<int>> adj_orange_pos_list = {
            {i - 1, j}, {i, j - 1}, {i + 1, j}, {i, j + 1}};
        for (vector<int> adj_orange_pos : adj_orange_pos_list) {
          int adj_i = adj_orange_pos[0], adj_j = adj_orange_pos[1];
          if ((adj_i < 0) || (adj_j < 0) || (adj_i >= grid.size()) ||
              (adj_j >= grid[0].size()) || (grid[adj_i][adj_j] == 0) ||
              (grid[adj_i][adj_j] == 2)) {
            // 触碰到网格边界了或者是已经被污染过了
            continue;
          } else {
            fresh_count--;
            grid[adj_i][adj_j] = 2;
            q.push({adj_i, adj_j});
          }
        }
      }
      result++;
    }
    if (fresh_count > 0) {
      return -1;
    } else {
      return result;
    }
  }
  bool pollution(vector<vector<int>> &grid, int i, int j) {
    if ((i < 0) || (j < 0) || (i >= grid.size()) || (j >= grid[0].size()) ||
        (grid[i][j] == 0) || (grid[i][j] == 2)) {
      return false;
    }
    return true;
  }
};
int main() {
  Solution solution;

  vector<vector<int>> grid = {{2, 1, 1}, {1, 1, 0}, {0, 1, 1}}; // 4
  int result = solution.orangesRotting(grid);
  cout << result << endl;
}
