#include "../binary_tree.h"
#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
  int maxDepth(TreeNode *root) {
    if (!root)
      return 0;
    int left_max = maxDepth(root->left);
    int right_max = maxDepth(root->right);
    return max(left_max, right_max) + 1;
  }
};

int main() {
  Solution solution;
  vector<int> layerorder = {3, 9, 20, NULL_FLAG, NULL_FLAG, 15, 7};
  TreeNode *root = buildTree(layerorder);
  int result = solution.maxDepth(root);
  cout << result << endl;
}
