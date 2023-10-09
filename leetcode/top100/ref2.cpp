// 多次反转实现旋转
#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
  // 反转区间内的元素
  void reverse(vector<int> &nums, int start, int end) {
    for (int l = start, r = end; l < r; l++, r--) {
      int tmp = nums[l];
      nums[l] = nums[r];
      nums[r] = tmp;
    }
  }

  // 多次反转实现旋转
  void rotate(vector<int> &nums, int k) {
    k = k % nums.size(); // 避免周期性旋转
    reverse(nums, 0, nums.size() - 1);
    reverse(nums, 0, k-1);
    reverse(nums, k, nums.size() - 1);
  }
};

int main() {
  vector<int> nums = {1, 2, 3, 4, 5, 6, 7};
  int k = 3;
  // [5,6,7,1,2,3,4]
  // vector<int> nums = {-1, -100, 3, 99};
  // int k = 2;
  // [3,99,-1,-100]
  Solution solution;
  solution.rotate(nums, k);
  for (int i : nums) {
    cout << i << " " << endl;
  }
}