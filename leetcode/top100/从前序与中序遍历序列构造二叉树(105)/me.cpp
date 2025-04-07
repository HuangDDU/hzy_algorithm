#include <iostream>
#include <set>
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

class Solution {
public:
  TreeNode *buildTree(vector<int> &preorder, vector<int> &inorder) {
    if (preorder.size() == 0) {
      return nullptr; // 递归终止条件
    } else {
      TreeNode *root = new TreeNode(preorder[0]); // 根结点
      if (preorder.size() == 1) {

        return root; // 递归终止条件
      } else {
        // 划分左右子树的前序和中序遍历序列
        vector<int> left_preorder, left_inorder, right_preorder, right_inorder;
        set<int> left_tree_value_set; // 左子树结点值集合
        int i = 0;
        for (; i < inorder.size(); i++) {
          if (inorder[i] == preorder[0])
            break;
          left_tree_value_set.insert(inorder[i]);
        }
        left_inorder = vector<int>(inorder.begin(), inorder.begin() + i);
        right_inorder = vector<int>(inorder.begin() + i + 1, inorder.end());
        i = 1;
        for (; i < preorder.size(); i++) {
          if (left_tree_value_set.find(preorder[i]) ==
              left_tree_value_set.end()) {
            break;
          }
        }
        left_preorder = vector<int>(preorder.begin() + 1, preorder.begin() + i);
        right_preorder = vector<int>(preorder.begin() + left_preorder.size() +1, preorder.end());
        // 递归构建左右子树
        root->left = buildTree(left_preorder, left_inorder);
        root->right = buildTree(right_preorder, right_inorder);
        return root;
      }
    }
  }
};

int main() {
  Solution solution;
    // vector<int> preorder = {3, 9, 20, 15, 7}, inorder = {9, 3, 15, 20, 7};
//   vector<int> preorder = {1, 2}, inorder = {2, 1};
  vector<int> preorder = {1, 2}, inorder = {1, 2};
  TreeNode *root = solution.buildTree(preorder, inorder);
  cout << root->val << endl;
}
