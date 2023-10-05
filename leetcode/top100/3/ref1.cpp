// 参考题解中排序后,去除连续重复的情况
#include <algorithm>
#include <iostream>
#include <set>
#include <vector>
using namespace std;

vector<vector<int>> threeSum(vector<int> &nums) {
  vector<vector<int>> res;        // 整体转化为vector
  sort(nums.begin(), nums.end()); // 整体排好序之后,达到去重的目的
  for (int i = 0; i < nums.size() - 2; i++) {
    if ((i > 0) && (nums[i] == nums[i - 1]))
      continue;
    for (int j = i + 1; j < nums.size() - 1; j++) {
      if ((j > i + 1) && (nums[j] == nums[j - 1]))
        continue;
      for (int k = j + 1; k < nums.size(); k++) {
        if ((k > j + 1) && (nums[k] == nums[k - 1]))
          continue;
        if (nums[i] + nums[j] + nums[k] == 0) {
          res.push_back({nums[i], nums[j], nums[k]});
          break; // 及时跳出最内层循环，避免重复添加
        }
      }
    }
  }
  return res;
}

int main() {
  // 1. 测试用例1
  // vector<int> nums = {-1,0,1,2,-1,-4};
  // [[-1,-1,2],[-1,0,1]]
  // 2. 执行时候的测试用例
  vector<int> nums = {3, 0, -2, -1, 1, 2};
  // [[-2,-1,3],[-2,0,2],[-1,0,1]]
  vector<vector<int>> res = threeSum(nums);
  // 这里简化输出
  for (vector<int> item : res) {
    for (int i : item) {
      cout << i << ",";
    }
    cout << endl;
  }
  return 0;
}
