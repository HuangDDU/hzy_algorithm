// 不打表了，只记住当前行的累加值，结果超时（与题解1一样）
#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
  int subarraySum(vector<int> &nums, int k) {
    int res = 0;
    int n = nums.size();
    // 二维打表，计算区间内的元素之和
    for (int i = 0; i < n; i++) {
      int tmp = 0;
      for (int j = i; j < n; j++) {
        tmp += nums[j];
        if (tmp == k) {
          res++;
        }
      }
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