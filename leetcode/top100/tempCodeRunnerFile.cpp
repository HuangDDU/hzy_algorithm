// 对于每个位置寻找雨水深度
#include <iostream>
#include <stack>
#include <vector>
using namespace std;

class Solution {
public:
  int trap(vector<int> &height) {
    int res = 0;
    stack<int>
        stk; // 存放当前i之前可以填雨水的位置，栈顶元素高度低于栈底元素高度
    int n = height.size();
    for (int i = 0; i < n; i++) {
      // 对于第i个位置之前，一层一层地填水
      while (!stk.empty() && height[i] > height[stk.top()]) {
        int top = stk.top(); // 填雨水的位置最低值
        stk.pop();
        if (stk.empty()) {
          break;
        }
        int left = stk.top();
        int currWidth = i - left + 1;
        int currHeight = min(height[left], height[i]) - height[top];
        res += currWidth * currHeight;
      }
    }
    return res;
  }
};

int main() {
  vector<int> height = {0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1};
  Solution s;
  cout << s.trap(height) << endl;
}
