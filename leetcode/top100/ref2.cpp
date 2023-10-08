// 使用map打表，双针移动时右指针不会回头
#include <iostream>
#include<string>
#include <unordered_map>
using namespace std;

class Solution {
public:
  bool check(unordered_map<char, int> m1, unordered_map<char, int> m2) {
    // 判断m1的字符表包含了m2字符表
    for (auto &item : m2) {
      if (m1[item.first] < item.second) {
        return false;
      }
    }
    return true;
  }

  string minWindow(string s, string t) {
    int start = -1, len = INT_MAX;
    unordered_map<char, int> s_sub_map, t_map; // s子串，t中字符表
    for (char ch : t) {
      t_map[ch]++;
    }
    int l = 0, r = -1; // [l,r]区间内的字符
    while (r < int(s.size())) {
      s_sub_map[s[++r]]++; // 移动右指针
      while (check(s_sub_map, t_map) && l <= r) {
        if (r - l + 1 < len) {
          len = r - l + 1;
          start = l;
        }
        s_sub_map[s[l]]++; // 移动左指针
      }
    }
    return start == -1 ? string() : s.substr(start, len);
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