#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
  vector<vector<int>> permute(vector<int> &nums) {
    vector<bool> visited = vector<bool>(nums.size(), false);
    return permute_recursive(nums, visited);
  }
  vector<vector<int>> permute_recursive(vector<int> &nums,
                                        vector<bool> visited) {
    vector<vector<int>> result;
    bool flag = true; // 标志对应的空列表
    for (int i = 0; i < nums.size(); i++) {
      if (!visited[i]) {
        flag = false;
        visited[i] = true; // 这里直接在visited上修改
        vector<vector<int>> tmp_result = permute_recursive(nums, visited);
        for (vector<int> tmp_result_item : tmp_result) {
          tmp_result_item.push_back(nums[i]);
          result.push_back(tmp_result_item);
        }
        visited[i] = false;
      }
    }
    if (flag) {
      // 递归终止条件
      return {{}};
    }
    return result;
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
