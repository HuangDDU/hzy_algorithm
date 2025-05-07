#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
  vector<vector<int>> result = {};
  vector<vector<int>> permute(vector<int> &nums) {
    vector<bool> visited = vector<bool>(nums.size(), false);
    permute_recursive(nums, visited, {});
    return result;
  }
  void permute_recursive(vector<int> &nums, vector<bool> visited,
                         vector<int> prefix) {
    if (prefix.size() == nums.size()) {
      // 递归停止条件
      result.push_back(prefix);
      return;
    }
    for (int i = 0; i < nums.size(); i++) {
      if (!visited[i]) {
        visited[i] = true; // 这里直接在visited上修改
        prefix.push_back(nums[i]);
        permute_recursive(nums, visited, prefix);
        visited[i] = false;
        prefix.pop_back();
      }
    }
  }
};
int main() {
  Solution solution;

  vector<int> nums = {1, 2, 3}; // false
  vector<vector<int>> result = solution.permute(nums);
  for (vector<int> result_item : result) {
    cout << "[";
    for (int i : result_item) {
      cout << i << ",";
    }
    cout << "]" << endl;
  }
}
