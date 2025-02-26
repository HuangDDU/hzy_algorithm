#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

vector<int> twoSum(vector<int> &nums, int target) {
  unordered_map<int, int> hashtable; // 键为已有值的补，值为已有值
  for (int i = 0; i < nums.size(); i++) {
    unordered_map<int, int>::iterator it = hashtable.find(target - nums[i]);
    if (it != hashtable.end()) {
      // 当前值为之前某个值的补，则返回当前值和之前值
      return {it->second, i};
    } else {
      // 当前值为新值，则保存键为当前值的补，值为当前值的键值对
      hashtable[nums[i]] = i;
    }
  }
  return {};
}

int main() {
  // 输入
  vector<int> nums = {2, 7, 11, 15};
  int target = 9;
  // 计算
  vector<int> result = twoSum(nums, target);
  // 输出
  for (int i = 0; i < result.size(); i++) {
    cout << result[i] << " ";
  }
  return 0;
}