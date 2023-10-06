// 确定每个位置的短板后双指针移动
#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
  int trap(vector<int> &height) {
    int res = 0;
    int left = 0, left_max=0, right = height.size() - 1,
        right_max=0; // 左右指针指向
    while (left <= right) {
      if (left_max < right_max) {
        // 此时left的短板为left_max
        if (height[left] >= left_max) {
          left_max = height[left];
        } else {
          res += left_max - height[left];
        }
        left++;
      } else {
        // 此时right的短板为right_max
        if (height[right] >= right_max) {
          right_max = height[right];
        } else {
          res += right_max - height[right];
        }
        right--;
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
