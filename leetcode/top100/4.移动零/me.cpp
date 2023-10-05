#include <iostream>
#include <vector>
using namespace std;

void moveZeroes(vector<int> &nums) {
  int zero_count = 0; // 记录当前位置之前的0元素个数
  // 第一遍扫描移动元素
  for (int i = 0; i < nums.size(); i++) {
    if (nums[i] == 0) {
      zero_count++;
    } else {
      nums[i - zero_count] = nums[i];
    }
  }
  // 第二遍把填0
  for (int i = nums.size() - zero_count; i < nums.size(); i++) {
    nums[i] = 0;
  }
}

int main() {
  vector<int> nums = {0, 1, 0, 3, 12};
  moveZeroes(nums);
  for (int i : nums) {
    cout << i << " " << endl;
  }
}