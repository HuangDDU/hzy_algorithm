// 双指针移动，map保留子串中字符下标
#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

class Solution {
 public:
  int lengthOfLongestSubstring(string s) {
    if (s.length() == 0) {
      return 0;
    }
    int result = 1;
    int left = 0;
    unordered_map<char, int> sub_str_map;  // 已有子串map字典
    sub_str_map[s[0]] = 0;
    for (int right = 1; right < s.length(); right++) {
      if (sub_str_map.find(s[right]) == sub_str_map.end()) {
        // 新字符，添加进map
        sub_str_map[s[right]] = right;
        if (sub_str_map.size() > result) {
          result = sub_str_map.size();
        }
      } else {
        // 遇到了重复字符，从重复位置的后一个位置开始，重新构建map
        left = sub_str_map[s[right]] + 1;
        sub_str_map.clear();
        for (int i = left; i <= right; i++) {
          sub_str_map[s[i]] = i;
        }
      }
    }
    return result;
  }
};

int main() {
  // string s = "abcabcbb";  // 3
  // string s = "au";  // 2
  string s = "aab";  // 2
  Solution solution;
  cout << solution.lengthOfLongestSubstring(s) << endl;
}
