// 字符转化为数组下标，打表记录字符出现的次数（由于版本问题编译不通过）
#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
  string minWindow(string s, string t) {
    string res;
    if(s.length()<t.length()) return ""; // 特殊情况直接返回，窗口构建会报错
    int n_s = s.length(), n_t = t.length();
    vector<int> s_count(52),
        t_count(52); // 大小写字母字符转化为下标,使用数组进行打表,这里注意ASCII表中大写字母为65~90，小写字母为97~122
    for (int i = 0; i < n_t; i++) {
      s_count[s[i] - 'A']++;
      t_count[t[i] - 'A']++;
    }
    for (int i = 0; i < n_s; i++) {
      // 外循环移动左指针
      // 初始化内循环需要的表
      if (i != 0) {
        s_count[s[i - 1] - 'A']--;
        s_count[s[i + n_t - 1] - 'A']++;
      }
      vector<int> s_count_tmp(s_count.begin(), s_count.end());
      for (int j = i + n_t; j <= n_s; j++) {
        // 内循环移动区间的右指针，比较区间内所有字符的次数
        // [i,j)
        s_count_tmp[s[j - 1] - 'A']++;
        bool flag = true;
        for (int k = 0; k < t_count.size(); k++) {
          if (s_count_tmp[k] < t_count[k]) {
            flag = false;
            break;
          }
        }
        if (flag) {
          // string tmp_res = s.substr(i, j-i);
          if (res.size() == 0) {
            res = s.substr(i, j - i); // 初次找到结果
          } else if (j - i < res.size()) {
            res = s.substr(i, j - i); // 找到新的结果
          }
          break;
        }
      }
    }
    return res;
  }
};

int main() {
  string s = "ADOBECODEBANC", t = "ABC";
  // "BANC"
  // string s = "a", t = "a";
  // string s = "a", t = "aa";
  Solution solution;
  string res = solution.minWindow(s, t);
  cout << res << endl;
}