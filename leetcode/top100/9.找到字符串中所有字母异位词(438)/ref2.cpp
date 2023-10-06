// 不需要比对两个数组表了,直接记录差异值
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

    vector<int> count(26); // 小写字母字符转化为下标,使用数组进行打表
    for (int i = 0; i < n_p; i++) {
      // 为+表示s子串多了该字符,为-表示s子串少了该字符,为0表示该字符次数相等
      count[s[i] - 'a']++;
      count[p[i] - 'a']--;
    }

    int differ = 0; // 差异字符的个数
    for (int i = 0; i < 26; i++) {
      if (count[i] != 0) {
        differ++;
      }
    }

    if (differ == 0) {
      res.push_back(0);
    }

    for (int i = 0; i < n_s - n_p; i++) {
      // 从[i, i+n_p)窗口移动到[i+1, i+np+1)窗口
      // 丢弃字符s[i]
      if (count[s[i] - 'a'] == 1) {
        differ--;
      } else if (count[s[i] - 'a'] == 0) {
        differ++;
      }
      count[s[i] - 'a']--;
      // 添加字符s[i+n_p]
      if (count[s[i + n_p] - 'a'] == -1) {
        differ--;
      } else if (count[s[i + n_p] - 'a'] == 0) {
        differ++;
      }
      count[s[i + n_p] - 'a']++;

      if (differ == 0) {
        res.push_back(i + 1);
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
