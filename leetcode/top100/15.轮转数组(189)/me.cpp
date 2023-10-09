// 借用辅助变量暂存并赋值
#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
  void rotate(vector<int> &nums, int k) {
    k = k % nums.size();                              // 避免周期性旋转
    vector<int> tmp_nums(nums.end() - k, nums.end()); // 辅助变量
    for (int i = nums.size() - 1; i >= k; i--) {
      // 从后往前扫描
      nums[i] = nums[i - k];
    }
    for (int i = 0; i < k; i++) {
      nums[i] = tmp_nums[i];
    }
  }
};

int main() {
  // vector<int> nums = {1, 2, 3, 4, 5, 6, 7};
  // int k = 3;
  // [5,6,7,1,2,3,4]
  vector<int> nums = {-1, -100, 3, 99};
  int k = 2;
  // [3,99,-1,-100]
  Solution solution;
  solution.rotate(nums, k);
  for (int i : nums) {
    cout << i << " " << endl;
  }
}