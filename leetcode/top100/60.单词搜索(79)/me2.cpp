#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
  bool exist(vector<vector<char>> &board, string word) {
    int m = board.size(), n = board[0].size();
    for (int i = 0; i < m; i++) {
      for (int j = 0; j < n; j++) {
        vector<vector<bool>> visited =
            vector<vector<bool>>(m, vector<bool>(n, false));
        bool tmp_result = search(board, word, i, j, visited, 0);
        if (tmp_result) {
          return true;
        }
      }
    }
    return false;
  }

  bool search(vector<vector<char>> board, string word, int i, int j,
              vector<vector<bool>> visited, int char_pos) {
    
    if ((i < 0) || (i >= board.size()) || (j < 0) || (j >= board[0].size())) {
      // 网格越界
      return false;
    }
    if (!(word[char_pos] == board[i][j])) {
      // 当前字符不匹配
      return false;
    }
    if (visited[i][j]) {
        // 访问过该结点
        return false;
    };
    if (char_pos==word.size()-1){
        // 这里要及时退出，不做额外搜索
        return true;
    }
    visited[i][j] = true;
    // 上下左右搜索
    vector<vector<int>> new_ij_vector = {
        {i - 1, j}, // 上
        {i + 1, j}, // 下
        {i, j - 1}, // 左
        {i, j + 1}, // 右
    };
    for (vector<int> new_ij : new_ij_vector) {
      bool tmp_result =
          search(board, word, new_ij[0], new_ij[1], visited, char_pos + 1);
      if (tmp_result) {
        return true;
      }
    }
    visited[i][j] = false;
    return false;
  }
};
int main() {
  Solution solution;
  //   vector<vector<char>> board = {
  //       {'A', 'B', 'C', 'E'}, {'S', 'F', 'C', 'S'}, {'A', 'D', 'E', 'E'}};
  //   string word = "ABCCED";
//   vector<vector<char>> board = {
//       {'A', 'B', 'C', 'E'}, 
//       {'S', 'F', 'C', 'S'}, 
//       {'A', 'D', 'E', 'E'}
//     };
//   string word = "ABCB";

  vector<vector<char>> board = {
      {'A', 'A', 'A', 'A', 'A', 'A'}, 
      {'A', 'A', 'A', 'A', 'A', 'A'},
      {'A', 'A', 'A', 'A', 'A', 'A'}, 
      {'A', 'A', 'A', 'A', 'A', 'A'},
      {'A', 'A', 'A', 'A', 'A', 'A'}, 
      {'A', 'A', 'A', 'A', 'A', 'A'}
    };
  string word = "AAAAAAAAAAAAAAB";

  bool result = solution.exist(board, word);
  cout << result;
}
