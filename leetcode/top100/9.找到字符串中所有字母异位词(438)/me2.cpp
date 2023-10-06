// 使用map打表s,p的字符集
#include <iostream>
#include <map>
#include <vector>
using namespace std;

class Solution {
public:
  vector<int> findAnagrams(string s, string p) {
    vector<int> res;
    int n_s = s.length(), n_p = p.length();
    if (n_s < n_p) {
      // 边界条件，直接返回
      return res;
    }
    map<char, int> s_m, p_m; // 打表记录各个字符出现的次数
    // 初始化
    for (int i = 0; i < 26; i++) {
      char ch = 'a' + i;
      p_m[ch] = 0;
      s_m[ch] = 0;
    }
    for (int i = 0; i < n_p; i++) {
      s_m[s[i]]++;
      p_m[p[i]]++;
    }
    for (int i = 0; i < n_s - n_p + 1; i++) {
      if (i != 0) {
        // 更新当前位置的表
        s_m[s[i - 1]]--;
        s_m[s[i + n_p -1]]++;
      }
      // 比对所有字符出现次数是否相同
      bool flag = true;
      for (int j = 0; j < 26; j++) {
        char ch = 'a' + j;
        if (p_m[ch] != s_m[ch]) {
          flag = false;
          break;
        }
      }
      if (flag) {
        res.push_back(i);
      }
    }
    return res;
  }
};

int main() {
  string s = "cbaebabacd", p = "abc"; // [0, 6]
  // string s = "aa", p = "bb"; // []
  Solution solution;
  vector<int> res = solution.findAnagrams(s, p);
  for (int i : res) {
    cout << i << endl;
  }
}
