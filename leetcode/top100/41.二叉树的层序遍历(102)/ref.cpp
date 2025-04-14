#include "../binary_tree.h"
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

class Solution {
public:
  vector<vector<int>> levelOrder(TreeNode *root) {
    if (!root)
      return {};
    vector<vector<int>> result;
    queue<TreeNode *> q; // 直接保存结点
    q.push(root);
    while (!q.empty()) {
      int tmp_size = q.size();
      // 一次性把当前层结点全部取出来
      vector<int> tmp_result;
      for (int i = 0; i < tmp_size; i++) {
        TreeNode *node = q.front();
        q.pop();
        tmp_result.push_back(node->val);
        if (node->left)
          q.push(node->left);
        if (node->right)
          q.push(node->right);
      }
      result.push_back(tmp_result);
    }
    return result;
  }
};

int main() {
  Solution solution;
  vector<int> layerorder = {3,         9,  20, NULL_FLAG,
                            NULL_FLAG, 15, 7}; // [[3],[9,20],[15,7]]
  TreeNode *root = buildTree(layerorder);
  vector<vector<int>> result = solution.levelOrder(root);
  for (vector<int> result_item : result) {
    for (int i : result_item) {
      cout << i << " ";
    }
    cout << endl;
  }
}
