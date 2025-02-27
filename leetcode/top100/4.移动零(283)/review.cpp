#include <iostream>
#include <vector>
using namespace std;

void moveZeroes(vector<int> &nums) {
  int non_zero_num = 0;  // 当前非零元素的数量，下一个非零元素放置的位置
  // 非零元素依次往前放
  for (int i = 0; i < nums.size(); i++) {
    if (nums[i] != 0) {
      nums[non_zero_num] = nums[i];
      non_zero_num++;
    }
  }
  // 非零元素放置完毕，后续位置填0
  for (int i = non_zero_num; i < nums.size(); i++) {
    nums[i] = 0;
  }
}

int main() {
  vector<int> nums = {0, 1, 0, 3, 12};  // 1,3,12,0,0
  moveZeroes(nums);
  for (int i : nums) {
    cout << i << " " << endl;
  }
}