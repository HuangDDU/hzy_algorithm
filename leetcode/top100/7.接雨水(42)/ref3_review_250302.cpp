// 对于每个位置寻找雨水深度，双指针移动较小者
#include <iostream>
#include <vector>
using namespace std;

class Solution {
 public:
  int trap(vector<int> &height) {
    int result = 0;
    int left = 1, right = height.size() - 2;
    int left_max = height[0], right_max = height[height.size() - 1];
    while (left <= right) {
      if (left_max < right_max) {
        left_max = max(left_max, height[left]);
        result += left_max - height[left];
        left++;
      } else {
        right_max = max(right_max, height[right]);
        result += right_max - height[right];
        right--;
      }
    }
    return result;
  }
};

int main() {
  vector<int> height = {0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1};  // 6
  Solution s;
  cout << s.trap(height) << endl;
}
