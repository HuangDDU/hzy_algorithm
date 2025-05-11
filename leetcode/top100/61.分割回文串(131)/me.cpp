#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
  vector<vector<vector<int>>> pos_result; // 分割区间的位置列表
  vector<vector<string>> partition(string s) {
    vector<vector<string>> result;
    partitionRecursive(s, 0, 0, {});
    for (vector<vector<int>> i : pos_result) {
      vector<string> tmp_result;
      for (vector<int> j : i) {
        tmp_result.push_back(s.substr(j[0], j[1] - j[0]));
      }
      result.push_back(tmp_result);
    }
    return result;
  }
  void
  partitionRecursive(string s,
                     int pos,                        // 当前字符位置
                     int begin,                      // 当前候选回文串的起始位置
                     vector<vector<int>> area_vector // 已经判断回文串区间[)
  ) {
    if (pos == s.size()) {
      // 递归终止条件
      if (isPalindrome(s, begin, pos)) {
        // 有效分割则添加到可行解里
        area_vector.push_back({begin, pos});
        pos_result.push_back(area_vector);
      }
      return;
    }

    // 延续老的回文串
    partitionRecursive(s, pos + 1, begin, area_vector);
    // 开辟新的回文串
    if (isPalindrome(s, begin, pos)) {
      area_vector.push_back({begin, pos});
      partitionRecursive(s, pos + 1, pos + 1, area_vector);
      area_vector.pop_back();
    }
  }

  bool isPalindrome(string s, int begin, int end) {
    // 字串[begin ,end)内是否是回文串
    end--;
    while (begin < end) {
      if (s[begin] != s[end]) {
        return false;
      }
      begin++;
      end--;
    }
    return true;
  }
};

int main() {
  Solution solution;
  string s = "aab";

  vector<vector<string>> result = solution.partition(s);
  for (vector<string> result_item : result) {
    cout << "[";
    for (string i : result_item) {
      cout << i << ",";
    }
    cout << "]" << endl;
  }
}
