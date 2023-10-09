// 使用结果数组存储前后缀数组
#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
  vector<int> productExceptSelf(vector<int> &nums) {
    int n = nums.size();
    vector<int> res(n, 1);
    // 分别存储前缀乘积和后缀乘积数组
    for (int i = 1; i < n; i++) {
      res[i] = res[i - 1] * nums[i - 1];
    }
    // 累计后缀 
    int suffix = 1;
    for (int i = n - 2; i >= 0; i--) {
      suffix = suffix * nums[i + 1];
      res[i] = res[i] * suffix;
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