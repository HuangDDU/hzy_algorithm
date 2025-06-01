#include <iostream>
#include <map>
#include <stack>

using namespace std;

class Solution {
public:
  string decodeString(string s) {
    string result;
    stack<string> st;
    for (char ch : s) {
      string ch_str(1, ch);
      if (ch_str == "]") {
        // 弹出字符
        // 弹出子串
        string sub_str = "";
        while (st.top() != "[") {
          sub_str = st.top() + sub_str;
          st.pop();
        }
        st.pop();
        // 弹出数值
        int num = 0, base = 1;
        while ((!st.empty()) && (st.top()[0] >= '0') && (st.top()[0] <= '9')) {
          num += (st.top()[0] - '0') * base;
          st.pop();
          base *= 10;
        }
        // 拼接重复指定次数的子串
        string sub_str_sum;
        for (int i = 0; i < num; i++) {
          sub_str_sum += sub_str;
        }
        st.push(sub_str_sum);
      } else {
        // 压入字符
        st.push(ch_str);
      }
    }
    while (!st.empty()) {
      result = st.top() + result;
      st.pop();
    }
    return result;
  }
};
int main() {
  Solution solution;
    string s = "3[a]2[bc]"; // aaabcbc
//   string s = "3[a2[c]]"; // accaccacc
//   string s ="100[leetcode]";
  string result = solution.decodeString(s);
  cout << result << endl;
}
