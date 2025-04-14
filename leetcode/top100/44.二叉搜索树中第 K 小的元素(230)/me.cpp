#include "../binary_tree.h"
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

class Solution {
public:
  int count = 0;
  int kthSmallest(TreeNode *root, int k) {
    if (!root)
      return -1;
    int result;

    int left_result = kthSmallest(root->left, k);
    if (left_result >= 0)
      return left_result; // 左子树找到了目标
    count++;
    if (count == k)
      return root->val; // 当前根节点为目标
    int right_result = kthSmallest(root->right, k);
    if (right_result >= 0)
      return right_result; // 右子树找到了目标
    return -1;
  }
};

int main() {
  Solution solution;
  //   vector<int> layerorder = {3, 1, 4, NULL_FLAG, 2};
  //   int k = 1;
  vector<int> layerorder = {5, 3, 6, 2, 4, NULL_FLAG, NULL_FLAG, 1};
  int k = 3;
  TreeNode *root = buildTree(layerorder);
  int result = solution.kthSmallest(root, k);
  cout << result << endl;
}
