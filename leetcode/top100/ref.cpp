// 对于每个位置寻找雨水深度
#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
  int trap(vector<int> &height) {
    int res = 0;
    int n = height.size();
    // 动态规划分别找每个位置的左边最大和右边最大，确定深度
    vector<int> left_max_array(n, 0), right_max_array(n, 0);
    for (int i = 1; i < n; i++) {
      left_max_array[i] = max(left_max_array[i - 1], height[i - 1]);
    }
    for (int i = n - 2; i > 0; i--) {
      right_max_array[i] = max(right_max_array[i + 1], height[i + 1]);
    }
    // 深度累加计算与水量
    for (int i = 0; i < n; i++) {
      int depth = min(left_max_array[i], right_max_array[i]);
      if (depth > height[i]) {
        res += depth - height[i];
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
