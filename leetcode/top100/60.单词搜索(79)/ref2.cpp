#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    int m,n;
  bool exist(vector<vector<char>> &board, string word) {
    m = board.size();
    n = board[0].size();
    for (int i = 0; i < m; i++) {
      for (int j = 0; j < n; j++) {
        if (search(board, word, i, j, 0)) {
          return true;
        }
      }
    }
    return false;
  }

  bool search(vector<vector<char>> board, string word, int i, int j,
              int char_pos) {

    if ((i < 0) || (i >= m) || (j < 0) || (j >= n) || board[i][j] != word[char_pos]) {
      // 网格越界，匹配错误，或者访问过
      return false;
    }
    if (char_pos == word.size() - 1) {
      // 这里要及时退出，不做额外搜索
      return true;
    }
    board[i][j] = '\0'; // 标记为访问过，后续回溯过程可以设置恢复
    // 上下左右搜索
    bool res = search(board, word, i + 1, j, char_pos + 1) ||
               search(board, word, i - 1, j, char_pos + 1) ||
               search(board, word, i, j + 1, char_pos + 1) ||
               search(board, word, i, j - 1, char_pos + 1);
    board[i][j] = word[char_pos];
    return res;
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
      {'A', 'A', 'A', 'A', 'A', 'A'}};
  string word = "AAAAAAAAAAAAAAB";

  bool result = solution.exist(board, word);
  cout << result;
}
