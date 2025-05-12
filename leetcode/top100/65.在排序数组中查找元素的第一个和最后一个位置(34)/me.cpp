#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
  vector<int> searchRange(vector<int> &nums, int target) {
    if (nums.size() == 0) {
      return {-1, -1};
    }
    // 先二分查找到一个元素
    int left = 0, right = nums.size() - 1;
    int mid;
    while (left <= right) {
      mid = (left + right) / 2;
      if (nums[mid] == target) {
        break;
      } else {
        if (nums[mid] < target) {
          left = mid + 1;
        } else {
          right = mid - 1;
        }
      }
    }
    if (nums[mid] != target) {
      // 没有查找到元素
      return {-1, -1};
    } else {
      // 查找到元素，向前向后扩展
      int l = mid, r = mid;
      while ((l > 0) && (nums[l - 1] == target)) {
        l--;
      }
      while ((r < nums.size() - 1) && (nums[r + 1] == target)) {
        r++;
      }
      return {l, r};
    }
  }
};
int main() {
  Solution solution;
  //   vector<int> nums = {5, 7, 7, 8, 8, 10};
  //   int target = 8; // {3, 4}
  //                   //   int target = 6; // {-1, -1}
  vector<int> nums = {1};
  int target = 1; // {0, 0}
  vector<int> result = solution.searchRange(nums, target);
  cout << result[0] << ',' << result[1] << endl;
}
