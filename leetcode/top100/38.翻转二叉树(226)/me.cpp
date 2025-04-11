#include "../binary_tree.h"
#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
  TreeNode *invertTree(TreeNode *root) {
    // 递归边界
    if (!root)
      return nullptr;
    // 分别对左右左子树反转
    invertTree(root->left);
    invertTree(root->right);
    TreeNode *left = root->left;
    root->left = root->right;
    root->right = left;
    return root;
  }
};

int main() {
  Solution solution;
  vector<int> layerorder = {4, 2, 7, 1, 3, 6, 9};
  TreeNode *root = buildTree(layerorder);
  TreeNode *new_root = solution.invertTree(root);
  cout << new_root->val << endl;
}
