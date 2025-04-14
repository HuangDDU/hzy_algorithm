#include "../binary_tree.h"
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

class Solution {
public:

  vector<int> rightSideView(TreeNode *root) {
    if (!root)
      return {};
    vector<int> result;
    queue<TreeNode *> q; // 队列直接保存结点
    q.push(root);
    while (!q.empty()) {
      int tmp_size = q.size();
      // 一次性把当前层结点全部取出来
      for (int i = 0; i < tmp_size; i++) {
        TreeNode *node = q.front();
        if (i == tmp_size - 1) {
          // 当前层的最后一个结点即为右视图
          result.push_back(node->val);
        }
        q.pop();
        if (node->left)
          q.push(node->left);
        if (node->right)
          q.push(node->right);
      }
    }
    return result;
  }
};

int main() {
  Solution solution;
  vector<int> layerorder = {1, 2, 3, NULL_FLAG, 5, NULL_FLAG, 4};
  TreeNode *root = buildTree(layerorder);
  vector<int> result = solution.rightSideView(root);
  for (int i : result) {
    cout << i << " " << endl;
  }
}
