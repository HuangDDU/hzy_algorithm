// 分治求解，
#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
  struct Status {
    // 分别表示靠近左边界区间最大，靠近右边界区间最大，整体区间最大，区间和
    int lSum, rSum, mSum, iSum;
  };

  // 两个区间合并为
  Status pushUp(Status l, Status r) {
    int iSum = l.iSum + r.iSum;
    int lSum = max(l.lSum, l.iSum + r.lSum);
    int rSum = max(r.rSum, r.iSum + l.rSum);
    int mSum = max(max(l.mSum, r.mSum), l.rSum + r.lSum);
    return (Status){lSum, rSum, mSum, iSum};
  };

  Status get(vector<int> &a, int l, int r) {
    // 递归边界
    if (l == r) {
      return (Status){a[l], a[l], a[l], a[l]};
    }
    // 分治
    int m = (l + r) / 2;
    Status lSub = get(a, l, m);
    Status rSub = get(a, m + 1, r);
    // 合并
    return pushUp(lSub, rSub);
  }

  int maxSubArray(vector<int> &nums) {
    Status s = get(nums, 0, nums.size() - 1);
    return s.mSum;
  }
};

int main() {

  vector<int> nums = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
  // [4,-1,2,1]
  // 6
  Solution solution;
  int res = solution.maxSubArray(nums);
  cout << res << endl;
}