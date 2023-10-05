#include <iostream>
#include <vector>
#include <set>
using namespace std;

int longestConsecutive(vector<int> &nums) {
  if (nums.size() == 0)
    return 0;
  set<int> nums_set(nums.begin(), nums.end()); // 使用集合来自动哈希,避免手动打表数组的问题
  int result = 1;
  for (int num : nums_set) {
    int tmp_num = num;
    int tmp_result = 1;
    if (nums_set.find((tmp_num - 1)) != nums_set.end()) {
      // 已经尝试过了,跳过
      continue;
    }
    while (nums_set.find((++tmp_num)) != nums_set.end()) {
      // 不断尝试+1
      tmp_result += 1;
    }
    result = max(result, tmp_result);
  }
  return result;
}

int main() {
  vector<int> nums = {100,4,200,1,3,2};
  cout << longestConsecutive(nums) << endl;
}