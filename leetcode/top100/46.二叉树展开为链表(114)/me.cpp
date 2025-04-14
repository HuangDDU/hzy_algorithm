#include "../binary_tree.h"
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

class Solution {
public:
  void flatten(TreeNode *root) {
    if (root) {
      flatten_recursive(root, NULL);
    }
  }

  void flatten_recursive(TreeNode *root, TreeNode *final_next) {
    if (root->left && root->right) {
      flatten_recursive(root->left, root->right);
      flatten_recursive(root->right, final_next);
      root->right = root->left;
      root->left = NULL;
    } else if (root->left && !root->right) {
      flatten_recursive(root->left, final_next);
      root->right = root->left;
      root->left = NULL;
    } else if (!root->left && root->right) {
      flatten_recursive(root->right, final_next);
    } else {
      root->right = final_next;
    }
  }
};

int main() {
  Solution solution;
  vector<int> layerorder = {1, 2, 5, 3, 4, NULL_FLAG, 6}; // {1, 2, 3, 4, 5, 6}
  TreeNode *root = buildTree(layerorder);
  solution.flatten(root);
  TreeNode *p = root;
  while (p) {
    cout << p->val << " ";
    p = p->right;
  }
}
