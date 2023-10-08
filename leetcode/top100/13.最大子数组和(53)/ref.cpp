// 两个变量分别存取扫过的最大区间和以结尾的最大区间和
#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
  int maxSubArray(vector<int> &nums) {
    int res = 0; // 现有区间内的最大子数组和
    int tmp = 0; // 以现在位置结尾的最大子数组和
    for (int i = 0; i < nums.size(); i++) {
      tmp = max(tmp+nums[i], nums[i]); // 这里限定要求区间必须至少一个元素
      res = max(res, tmp);
    }
    return res;
  }
};

int main() {

  vector<int> nums = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
  // [4,-1,2,1]
  // 6
  Solution solution;
  int res = solution.maxSubArray(nums);
  cout << res<< endl;
}