#include "../binary_tree.h"
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

class Solution {
public:
  int diameterOfBinaryTree(TreeNode *root) { return getDiameter(root)[0]-1; }

  vector<int> getDiameter(TreeNode *root) {
    // 递归获得直径上的节点数，返回值分别为最大的直径（当前根节点不一定为端点）和的树高度（当前结点为端点）
    if (!root)
      return {0, 0};
    vector<int> left_result = getDiameter(root->left);
    vector<int> right_result = getDiameter(root->right);
    // 新直径比较以根节点为中间的路径的直径与左右子树直径，选择较大者
    int diameter = (left_result[1] + right_result[1] + 1);
    diameter = max(diameter, left_result[0]);
    diameter = max(diameter, right_result[0]);
    // 新高度为子树高度较大者+1
    int height = max(left_result[1], right_result[1]) + 1;
    return {diameter, height};
  }
};

int main() {
  Solution solution;
  vector<int> layerorder = {1, 2, 3, 4, 5};
  TreeNode *root = buildTree(layerorder);
  int result = solution.diameterOfBinaryTree(root);
  cout << result << endl;
}
