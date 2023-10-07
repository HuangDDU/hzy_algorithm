// 打表记录前缀和出现的次数，确定前缀区间
#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

class Solution {
public:
  int subarraySum(vector<int> &nums, int k) {
    int res = 0;
    int n = nums.size();
    int pre = 0; // 累计的前缀和
    unordered_map<int, int> mp;
    mp[0] = 1; // 初始没有元素前缀和为0
    for (int i = 0; i < n; i++) {
      pre += nums[i];
      if (mp.find(pre - k) != mp.end()) {
        res += mp[pre - k]; // 确定前缀区间的个数
      }
      mp[pre]++;
    }
    return res;
  }
};

int main() {
  vector<int> nums = {1, 2, 3};
  int k = 3;
  // 2
  Solution solution;
  int res = solution.subarraySum(nums, k);
  cout << res << endl;
}