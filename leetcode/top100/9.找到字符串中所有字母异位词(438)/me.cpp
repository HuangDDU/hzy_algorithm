// 暴力求解所有位置的子串、排序、对比
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
  vector<int> findAnagrams(string s, string p) {
    sort(p.begin(), p.end()); // 字符排序
    int n_s = s.length(), n_p = p.length();
    vector<int> res;
    for (int i = 0; i < s.length(); i++) {
      // 每个位置开始的子串，排序
      string tmp_p = s.substr(i, n_p);
      sort(tmp_p.begin(), tmp_p.end());
      if (tmp_p == p) {
        res.push_back(i);
      }
    }
    return res;
  }
};

int main() {
  string s = "cbaebabacd", p = "abc";
  Solution solution;
  vector<int> res = solution.findAnagrams(s, p);
  for(int i : res){
    cout << i << endl;
  }
}