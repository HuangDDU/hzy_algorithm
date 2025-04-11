#include "../binary_tree.h"
#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
  vector<int> inorderTraversal(TreeNode *root) {

    vector<int> result;
    // 左
    vector<int> left_result = inorderTraversal(root->left);
    result.insert(result.end(), left_result.begin(), left_result.end());
    // 中
    result.push_back(root->val);
    // 右
    vector<int> right_result = inorderTraversal(root->right);
    result.insert(result.end(), right_result.begin(), right_result.end());

    return result;
  }
};

int main() {
  Solution solution;
  vector<int> layerorder = {1, NULL, 2, 3};
  TreeNode *root = buildTree(layerorder);
  vector<int> result = solution.inorderTraversal(root);
  for (int i : result) {
    cout << i << " " << endl;
  }
}
