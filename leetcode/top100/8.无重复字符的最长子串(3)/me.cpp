// 纯暴力，遍历所有的子串
#include <iostream>
#include <string>
#include <unordered_set>
using namespace std;

class Solution {
public:
  int lengthOfLongestSubstring(string s) {
    if (s.length() == 0) {
      return 0;
    }
    int res = 1;
    int n = s.length();
    for (int i = 0; i < n; i++) {
      for (int j = i + res; j <= n; j++) {
        // 第二层循环内部只看大于现有最长子串长度的子串s[i,j)
        string sub_s = s.substr(i, j - i);
        unordered_set<char> sub_s_set(sub_s.begin(),
                                      sub_s.end()); // 字符串转化为字符集合，无序集合效率更高
        if (sub_s_set.size() == sub_s.length()) {
          res = j - i;
        } else {
          // 出现了重复字符，跳出内层循环
          break;
        }
      }
    }
    return res;
  }
};

int main() {
  string s = "abcabcbb";
  Solution solution;
  cout << solution.lengthOfLongestSubstring(s) << endl;
}
