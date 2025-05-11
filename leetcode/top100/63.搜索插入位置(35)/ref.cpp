
#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
  int searchInsert(vector<int> &nums, int target) {
    int n = nums.size();
    int left = 0, right = n - 1; // 使用闭区间，可以简化后续逻辑
    int mid;
    while (left <= right) {
      mid = (right + left) / 2;
      if (target == nums[mid]) {
        return mid;
      }
      if (target < nums[mid]) {
        right = mid - 1;
      } else {
        left = mid + 1;
      }
    }
    return left; // 精确到一个位置后，在当前位置添加或在后一个位置添加
  }
};
int main() {
  Solution solution;
  vector<int> nums = {1, 3, 5, 6};
  //   int taget = 5; // 2
  // int taget = 2; // 1
  // int taget = 6; // 3
  int taget = 7; // 4
  int result = solution.searchInsert(nums, taget);
  cout << result << endl;
}
