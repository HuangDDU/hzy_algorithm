#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;

vector<vector<string>> groupAnagrams(vector<string> &strs) {

  vector<vector<string>> result;
  map<string, vector<string>> mp;

  for (string str : strs) {
    string key = str;
    sort(key.begin(), key.end()); // 排序后作为键名
    mp[key].push_back(str);       // 初始默认为空vector，直接push_back
  }

  for (map<string, vector<string>>::iterator it = mp.begin(); it != mp.end();
       ++it) {
    result.emplace_back(it->second);
  }
  return result;
}

int main() {
  vector<string> strs = {"eat", "tea", "tan", "ate", "nat", "bat"};
  vector<vector<string>> result = groupAnagrams(strs);
  // 这里简化输出，
  for (vector<string> result_item : result) {
    for (string str : result_item) {
      cout << str << ",";
    }
    cout << endl;
  }
  return 0;
}
