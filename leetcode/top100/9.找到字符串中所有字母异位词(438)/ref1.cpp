// 使用数组进行字符打表,构建和索引更快
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
    vector<int> s_count(26),
        p_count(26); // 小写字母字符转化为下标,使用数组进行打表
    for (int i = 0; i < n_p; i++) {
      s_count[s[i] - 'a']++;
      p_count[p[i] - 'a']++;
    }
    for (int i = 0; i < n_s - n_p + 1; i++) {
      if (i != 0) {
        // 更新当前位置的表
        s_count[s[i - 1] - 'a']--;
        s_count[s[i + n_p - 1] - 'a']++;
      }
      bool flag = true;
      if (s_count == p_count) {
        // 使用数组的表,可以直接比较
        res.push_back(i);
      }
    }
    return res;
  }
};

int main() {
  string s = "cbaebabacd", p = "abc"; // [0, 6]
                                      //   string s = "aa", p = "bb"; // []
  Solution solution;
  vector<int> res = solution.findAnagrams(s, p);
  for (int i : res) {
    cout << i << endl;
  }
}
