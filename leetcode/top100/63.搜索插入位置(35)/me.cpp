#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
  int searchInsert(vector<int> &nums, int target) {
    int left = 0, right = nums.size(); // [left ,right)区间内的二分查找
    int mid;
    while (left < right) {
      mid = (left + right) / 2;
      if (nums[mid] == target) {
        // 找到元素
        return mid;
      }
      if (right == left + 1) {
        // 此时只有一个元素，但还是不对应，则说明元素不存在，需要选择手动添加的位置
        if(target<nums[left]){
            return left;
        }else{
            return right;
        }
      } else if (nums[mid] < target) {
        left = (left + right) / 2;
      } else if (nums[mid] > target) {
        right = (left + right) / 2;
      }
    }
    return -1; // 不会执行这一步
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
