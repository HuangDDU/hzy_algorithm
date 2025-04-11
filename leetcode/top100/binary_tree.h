#include <deque>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

// 树结点定义
// Definition for a binary tree node.
struct TreeNode {
  int val;
  TreeNode *left;
  TreeNode *right;
  TreeNode() : val(0), left(nullptr), right(nullptr) {}
  TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
  TreeNode(int x, TreeNode *left, TreeNode *right)
      : val(x), left(left), right(right) {}
};

int NULL_FLAG = -101;
TreeNode *buildTree(vector<int> &layerorder) {
  if (layerorder.size() == 0) {
    return nullptr;
  }
  // layerorder; // 待添加结点
  queue<TreeNode *> q; // 待添加左右子树的结点
  TreeNode *head = new TreeNode(layerorder[0]);
  bool is_left=true; // 在左节点添加
  q.push(head);
  for (int i = 1; i < layerorder.size(); i++) {
    int value = layerorder[i];
    // 从队列头部提取结点添加左右左子树
    if (!(value==NULL_FLAG)) {
        TreeNode *node = q.front();
        TreeNode *new_node = new TreeNode(value);
        if (is_left) {
            node->left = new_node;
          } else {
            node->right = new_node;
          }
          q.push(new_node);
    }
    // 队列头部结点已经添加过了
    if(!is_left){
        q.pop();
    }
    is_left = !is_left; // 调节左右指针
  }
  return head;
}

// int main() {
// //   vector<int> layerorder = {1, NULL_FLAG, 2, 3}; // 94题目
//   vector<int> layerorder = {3,9,20,NULL_FLAG,NULL_FLAG,15,7}; // 104题目
//   TreeNode *root = buildTree(layerorder);
//   cout << root->val << endl;
// }
