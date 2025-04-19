#include "../binary_tree.h"
#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
  bool isSymmetric(TreeNode *root) {
    if (root) {
      return isSymmetricRecursive(root->left, root->right);
    } else {
      return true; // 空结点直接认为是对称的
    }
  }

  bool isSymmetricRecursive(TreeNode *lr, TreeNode *rr) {
    // 递归终止条件
    if (!lr && !rr) {
      return true;
    } else if ((!lr && rr) || (lr && !rr)) {
      return false;
    }
    // 经过后续所有的判断才被认为是对称的
    // 结点内数值相等
    if (!(lr->val == rr->val)) {
      return false;
    }
    // lr的左子树与rr的右子树对称，lr的右子树与rr的左左子树对称
    bool out_result = isSymmetricRecursive(lr->left, rr->right);
    if (!out_result) {
      return false;
    }
    bool inner_result = isSymmetricRecursive(lr->right, rr->left);
    if (!inner_result) {
      return false;
    }
    return true;
  }
};

int main() {
  Solution solution;
//   vector<int> layerorder = {1, 2, 2, 3, 4, 4, 3};                 // Trye
  vector<int> layerorder = {1, 2, 2, NULL_FLAG, 3, NULL_FLAG, 3}; // False
  TreeNode *root = buildTree(layerorder);
  bool result = solution.isSymmetric(root);
  cout << result << endl;
}
