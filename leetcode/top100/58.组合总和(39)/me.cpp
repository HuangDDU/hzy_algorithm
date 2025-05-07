#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
  vector<vector<int>> result;
  vector<vector<int>> combinationSum(vector<int> &candidates, int target) {
    combinationSumRecursive(candidates, target, {}, 0);
    return result;
  }
  void combinationSumRecursive(vector<int> &candidates, int target,
                               vector<int> prefix, int sum) {
    if (sum >= target) {
      // 限界
      if (sum == target) {
        // 可行解
        result.push_back(prefix);
      }
      return;
    }
    for (int candidate : candidates) {
      if ((prefix.size() > 0) && (candidate < prefix.back())) {
        // 限制要求递增排列组合内元素
        continue;
      }
      prefix.push_back(candidate);
      sum += candidate;
      combinationSumRecursive(candidates, target, prefix, sum);
      prefix.pop_back();
      sum -= candidate;
    }
  }
};
int main() {
  Solution solution;
  vector<int> candidates = {2, 3, 6, 7};
  int target = 7; // [[2,2,3],[7]]
  vector<vector<int>> result = solution.combinationSum(candidates, target);
  for (vector<int> result_item : result) {
    cout << "[";
    for (int i : result_item) {
      cout << i << ",";
    }
    cout << "]" << endl;
  }
}
