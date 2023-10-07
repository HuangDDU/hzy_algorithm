// 二维打表，计算区间内的元素之和，结果空间不够
#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
  int subarraySum(vector<int> &nums, int k) {
    int res = 0;
    int n = nums.size();
    // 二维打表，计算区间内的元素之和
    vector<vector<int>> v_table(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++) {
      for (int j = i; j < n; j++) {
        if (i == j) {
          // 只有一个元素
          v_table[i][j] = nums[i];
        } else {
          // 区间内元素累加
          v_table[i][j] = v_table[i][j - 1] + nums[j];
        }
      }
    }
    // 扫描表
    for (int i = 0; i < n; i++) {
      for (int j = i; j < n; j++) {
        if (v_table[i][j] == k) {
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