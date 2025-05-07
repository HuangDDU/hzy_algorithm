#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
  vector<vector<int>> subsets(vector<int> &nums) {
    return subsets_recurve(nums, 0);
  }
  vector<vector<int>> subsets_recurve(vector<int> &nums, int pos = 0) {
    if (pos >= nums.size()) {
      return {{}};
    }
    vector<vector<int>> tmp_result = subsets_recurve(nums, pos + 1);
    vector<vector<int>> result(tmp_result.begin(), tmp_result.end());
    for (vector<int> tmp_result_item : tmp_result) {
      tmp_result_item.push_back(nums[pos]);
      result.push_back(tmp_result_item);
    }
    return result;
  }
};
int main() {
  Solution solution;

  vector<int> nums = {1, 2, 3}; // false
  vector<vector<int>> result = solution.subsets(nums);
  for (vector<int> result_item : result) {
    cout << "[";
    for (int i : result_item) {
      cout << i << ",";
    }
    cout << "]" << endl;
  }
}
