#include "../binary_tree.h"
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

class Solution {
public:
  bool isValidBST(TreeNode *root) {
    return isValidRecursiveBST(root).first;
    ;
  }
  pair<bool, pair<int, int>> isValidRecursiveBST(TreeNode *root) {
    pair<bool, pair<int, int>> left_result, right_result;
    if (root->left) {
      left_result = isValidRecursiveBST(root->left);
    } else {
      left_result = make_pair(true, make_pair(root->val, root->val));
    }
    if (root->right) {
      right_result = isValidRecursiveBST(root->right);
    } else {
      right_result = make_pair(true, make_pair(root->val, root->val));
    }
    int left_min = left_result.second.first,
        left_max = left_result.second.second;
    int right_min = right_result.second.first,
        right_max = right_result.second.second;
    if (left_result.first && right_result.first &&
        ((left_max <= root->val) && (root->val <= right_min))) {
      return make_pair(true, make_pair(left_min, left_max));
    } else {
      return make_pair(false, make_pair(left_min, right_max));
    }
  }
};

int main() {
  Solution solution;
  vector<int> layerorder = {5, 1, 4, NULL_FLAG, NULL_FLAG, 3, 6};
  TreeNode *root = buildTree(layerorder);
  bool result = solution.isValidBST(root);
  cout << result << endl;
}
