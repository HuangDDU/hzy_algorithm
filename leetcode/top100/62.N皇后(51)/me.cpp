#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
  vector<vector<int>> pos_result;

  vector<vector<string>> solveNQueens(int n) {
    vector<vector<string>> result;
    // 先计算可行解的列位置
    solveQuuenesRecursive(n, 0, {});
    // 再计算字符串列表
    for (vector<int> pos_vector : pos_result) {
      vector<string> tmp_result;
      for (int pos : pos_vector) {
        string s(n, '.');
        s[pos] = 'Q';
        tmp_result.push_back(s);
      }
      result.push_back(tmp_result);
    }
    return result;
  }

  void solveQuuenesRecursive(int n,                 // 总行数
                             int row,               // 当前所在行
                             vector<int> pos_vector // 已安排的行所在列位置
  ) {
    if (row == n) {
      // 终止条件，已经完成所有皇后的设置
      pos_result.push_back(pos_vector);
    }
    for (int col = 0; col < n; col++) {
      bool attack_flag = false;
      for (int pre_row = 0; pre_row < pos_vector.size(); pre_row++) {
        int pre_col = pos_vector[pre_row];
        if (isAttack(pre_row, pre_col, row, col)) {
          // 限界条件：受到攻击
          attack_flag = true;
          break;
        }
      }
      if (!attack_flag) {
        // 继续搜索
        pos_vector.push_back(col);
        solveQuuenesRecursive(n, row + 1, pos_vector);
        pos_vector.pop_back();
      }
    }
  }

  bool isAttack(int row1, int col1, int row2, int col2) {
    if ((row1 == row2) || (col1 == col2)) {
      // 直线攻击
      return true;
    }
    if (abs(row1 - row2) == abs(col1 - col2)) {
      // 斜线攻击
      return true;
    }
    return false;
  }
};
int main() {
  Solution solution;
  int n = 4;
  vector<vector<string>> result = solution.solveNQueens(n);
  for (vector<string> result_item : result) {
    cout << "[";
    for (string i : result_item) {
      cout << i << ",";
    }
    cout << "]" << endl;
  }
}
