#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
  vector<string> result;
  vector<string> letterCombinations(string digits) {
    if (digits.size() == 0) {
      return {};
    }
    letterCombinations_recursive(digits, 0, "");
    return result;
  }
  void letterCombinations_recursive(string digits, int pos, string prefix) {
    if (pos == digits.size()) {
      result.push_back(prefix);
      return;
    }
    for (int i = 0; i < 3; i++) {
      char letter = 'a' + 3 * (digits[pos] - '2') + i; // 构造新的字符
      letterCombinations_recursive(digits, pos + 1, prefix + letter);
    }
  }
};
int main() {
  Solution solution;

//   string digits = "23"; // ["ad","ae","af","bd","be","bf","cd","ce","cf"]
  string digits = "7"; // ["p","q","r"]
  vector<string> result = solution.letterCombinations(digits);
  for (string result_item : result) {
    cout << result_item << ", ";
  }
}
