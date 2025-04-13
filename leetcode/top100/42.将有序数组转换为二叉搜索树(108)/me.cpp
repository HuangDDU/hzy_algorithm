#include "../binary_tree.h"
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

class Solution {
public:
  TreeNode *sortedArrayToBST(vector<int> &nums) {
    return getBST(nums, 0, nums.size());
  }
  TreeNode *getBST(vector<int> &nums, int start, int end) {
    // [start, end)区间内构造子树
    if (start >= end)
      return NULL;
    //   找中点划分构建左右子树
    int mid = (start + end) / 2;
    TreeNode *root = new TreeNode(nums[mid]);
    root->left = getBST(nums, start, mid);
    root->right = getBST(nums, mid + 1, end);
    return root;
  }
};

int main() {
  Solution solution;
  vector<int> nums = {-10, -3, 0, 5, 9};
  TreeNode *root = solution.sortedArrayToBST(nums);
  cout << root->val << endl;
}
