// 分别存储前缀乘积和后缀乘积数组,然后使用
#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
  vector<int> productExceptSelf(vector<int> &nums) {
    int n = nums.size();
    vector<int> res(n);
    // 分别存储前缀乘积和后缀乘积数组
    vector<int> prefix(n, 1), suffix(n, 1);
    for (int i = 1; i < n; i++) {
      prefix[i] = prefix[i - 1] * nums[i - 1];
    }
    for (int i = n - 2; i >= 0; i--) {
      suffix[i] = suffix[i + 1] * nums[i + 1];
    }
    for (int i = 0; i < n; i++) {
      res[i] = prefix[i] * suffix[i];
    }
    return res;
  }
};

int main() {
  vector<int> nums = {1, 2, 3, 4};
  // [24,12,8,6]
  // vector<int> nums = {-1, 1, 0, -3, 3};
  //  [0,0,9,0,0]
  Solution solution;
  vector<int> res = solution.productExceptSelf(nums);
  for (int i : res) {
    cout << i << " " << endl;
  }
}