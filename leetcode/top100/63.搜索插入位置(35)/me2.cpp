#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
  int searchInsert(vector<int> &nums, int target) {
    return searchInsertRecursive(nums, 0, nums.size(), target);
  }
  int searchInsertRecursive(vector<int> &nums, int left, int right,
                            int target) {
    int mid = (left + right) / 2;
    if (nums[mid] == target) {
      return mid;
    } else if(right == left + 1){
    if (target < nums[left]) {
        return left;
        } else {
        return right;
        }
    } 
    else {
      if (nums[mid] < target) {
        left = (left + right) / 2;
      } else if (nums[mid] > target) {
        right = (left + right) / 2;
      }
      return searchInsertRecursive(nums, left, right, target);
    }
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
