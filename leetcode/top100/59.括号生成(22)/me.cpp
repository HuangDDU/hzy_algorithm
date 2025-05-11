#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
  vector<string> result;
  vector<string> generateParenthesis(int n) {
    generateParenthesisRecursive(n, 0, 0, "");
    return result;
  }
  void generateParenthesisRecursive(int n, int left_over, int pair, string prefix) {
    if ((pair >= n) || (left_over < 0) || (left_over > n)) {
      // 限界
      if ((pair == n) && (left_over == 0)) {
        // 可行解
        result.push_back(prefix);
      }
      return;
    }
    // 二叉搜索树
    generateParenthesisRecursive(n, left_over + 1, pair, prefix + '(');
    generateParenthesisRecursive(n, left_over - 1, pair + 1, prefix + ')');
  }
};
int main() {
  Solution solution;
  int n = 3; // ["((()))","(()())","(())()","()(())","()()()"]
  vector<string> result = solution.generateParenthesis(n);
  for (string result_item : result) {
    cout << result_item << ", ";
  }
}
