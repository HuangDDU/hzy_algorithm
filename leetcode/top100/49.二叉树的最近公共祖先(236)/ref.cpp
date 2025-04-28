#include "../binary_tree.h"
#include <iostream>
#include <stack>
using namespace std;

class Solution {
public:
  TreeNode *lowestCommonAncestor(TreeNode *root, TreeNode *p, TreeNode *q) {
    // TODO: 直接递归，返回结果代表不同含义
    return root;
  }
};
int main() {
  Solution solution;
//   vector<int> layerorder = {3, 5, 1, 6, 2, 0, 8, NULL_FLAG, NULL_FLAG, 7, 4};
//   TreeNode *root = buildTree(layerorder);
//   TreeNode *p = root->left;  // 5
//   TreeNode *q = root->right; // 1

  vector<int> layerorder = {3, 5, 1, 6, 2, 0, 8, NULL_FLAG, NULL_FLAG, 7, 4};
  TreeNode *root = buildTree(layerorder);
  TreeNode *p = root->left;               // 5
  TreeNode *q = root->left->right->right; // 4
  TreeNode *result = solution.lowestCommonAncestor(root, p, q);
  cout << result->val << endl;
}
