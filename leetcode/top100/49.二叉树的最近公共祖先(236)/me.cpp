#include "../binary_tree.h"
#include <iostream>
#include <stack>
using namespace std;

class Solution {
public:
  TreeNode *lowestCommonAncestor(TreeNode *root, TreeNode *p, TreeNode *q) {
    // 查找两个结点路径，返回结果用栈保存，跟结点在栈顶
    stack<TreeNode *> p_path = findNodePath(root, p);
    stack<TreeNode *> q_path = findNodePath(root, q);
    TreeNode *result = root;
    while (!p_path.empty() && !q_path.empty()) {
      if (p_path.top() == q_path.top()) {
        result = p_path.top();
        p_path.pop();
        q_path.pop();
      } else {
        break;
      }
    }
    return result;
  }
  stack<TreeNode *> findNodePath(TreeNode *root, TreeNode *p) {
    // 返回目标结点到根节点的路径，返回结果用栈保存，如果没有中找到，则返回空栈
    if (!root) {
      return stack<TreeNode *>();
    }
    if (root == p) {
      return stack<TreeNode *>({p});
    }
    stack<TreeNode *> left_result = findNodePath(root->left, p);
    if (!(left_result.size() == 0)) {
      left_result.push(root);
      return left_result;
    }
    stack<TreeNode *> right_result = findNodePath(root->right, p);
    if (!(right_result.size() == 0)) {
      right_result.push(root);
      return right_result;
    }
    return stack<TreeNode *>();
  }
};
int main() {
  Solution solution;
//   vector<int> layerorder = {3, 5, 1, 6, 2, 0, 8, NULL_FLAG, NULL_FLAG, 7, 4};
//   TreeNode *root = buildTree(layerorder);
//   TreeNode *p = root->left;  // 5
//   TreeNode *q = root->right; // 1

  vector<int> layerorder = {3, 5, 1, 6, 2, 0, 8, NULL_FLAG, NULL_FLAG, 7, 4};
  TreeNode *root = buildTree(layerorder);
  TreeNode *p = root->left;               // 5
  TreeNode *q = root->left->right->right; // 4
  TreeNode *result = solution.lowestCommonAncestor(root, p, q);
  cout << result->val << endl;
}
