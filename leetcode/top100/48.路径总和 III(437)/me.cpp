#include "../binary_tree.h"
#include <iostream>
#include <set>
#include <vector>

using namespace std;

class Solution {
public:
  int count;
  int pathSum(TreeNode *root, int targetSum) {
    count = 0;
    pathSumRecurive(root, targetSum, {});
    return count;
  }

  void pathSumRecurive(TreeNode *root, int targetSum, vector<long> accumlate) {
    // 第i层结点，传入的accumlate中有i-1个, 前i-1个祖先组成的累计路径长度
    if (!root)
      return;

    vector<long> new_accumlate;
    for (long value : accumlate) {
      new_accumlate.push_back(value + root->val);
    }
    new_accumlate.push_back(root->val);
    for (long value : new_accumlate) {
      if (value == targetSum) {
        count++;
      }
    }
    pathSumRecurive(root->left, targetSum, new_accumlate);
    pathSumRecurive(root->right, targetSum, new_accumlate);
  }
};

int main() {
  Solution solution;
  int targetSum = 8;
  vector<int> layerorder = {10, 5, -3, 3,         2, NULL_FLAG,
                            11, 3, -2, NULL_FLAG, 1}; // 3
  TreeNode *root = buildTree(layerorder);
  int result = solution.pathSum(root, targetSum);
  cout << result << endl;
}
