// 对于每个位置寻找雨水深度，分别计算左右最大值，取较小值减去当前高度即可
#include <iostream>
#include <vector>
using namespace std;

class Solution {
 public:
  int trap(vector<int> &height) {
    int result = 0;
    int n = height.size();
    vector<int> left_max(n), right_max(n);
    // 分别计算从左开始和从右开始扫描最大值
    for (int i = 1; i < n; i++) {
      left_max[i] = max(left_max[i - 1], height[i - 1]);
    }
    for (int i = n - 2; i >= 0; i--) {
      right_max[i] = max(right_max[i + 1], height[i + 1]);
    }
    // 计算每个位置的雨水深度，取左右最大值的较小值减去当前高度
    for (int i = 0; i < n; i++) {
      result += max(0, min(left_max[i], right_max[i]) - height[i]);
    }
    return result;
  }
};

int main() {
  vector<int> height = {0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1};  // 6
  Solution s;
  cout << s.trap(height) << endl;
}
