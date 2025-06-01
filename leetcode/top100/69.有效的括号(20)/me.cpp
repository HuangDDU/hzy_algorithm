#include <iostream>
#include <map>
#include <stack>

using namespace std;

class Solution {
public:
  bool isValid(string s) {
    stack<char> st;
    // 括号字符映射关系
    map<char, char> ch_map;
    ch_map[')'] = '(';
    ch_map[']'] = '[';
    ch_map['}'] = '{';

    for (char ch : s) {
      if (ch_map.find(ch) != ch_map.end()) {
        // 右括号看是否与栈顶元素匹配映射
        if ((st.empty()) || (st.top() != ch_map[ch])) {
          return false;
        }
        st.pop();
      } else {
        st.push(ch);
      }
    }
    // 最后栈为空表示所有括号匹配完成
    return st.empty();
  }
};
int main() {
  Solution solution;
  string s = "()[]{}";
  bool result = solution.isValid(s);
  cout << result << endl;
}
