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
      // if (tmp + nums[i] > 0) {
      //   tmp = tmp + nums[i]; // 扩展区间内的元素
      // } else {
      //   tmp = 0; // 不要任何元素
      // }
      tmp = max(tmp+nums[i], 0); //简写 
      res = max(res, tmp);
    }

    // 特殊情况处理，全都是负值
    if(res == 0){
      res = nums[0];
      for (int i = 1; i < nums.size(); i++){
        res = max(res, nums[i]);
      }
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