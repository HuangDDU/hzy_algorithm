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
    queue<pair<TreeNode *, int>> q; // 队列中元素分别为结点和所在的层数
    int prervious_layer = 0;
    q.push(make_pair(root, 1));
    while (!q.empty()) {
      pair<TreeNode *, int> tmp_pair = q.front();
      q.pop();
      TreeNode *node = tmp_pair.first;
      int layer = tmp_pair.second;
      if (layer > prervious_layer) {
        //   碰到新的层则添加新的空数组
        result.push_back({});
        prervious_layer = layer;
      }
      result[result.size() - 1].push_back(node->val);
      if (node->left)
        q.push(make_pair(node->left, layer + 1));
      if (node->right)
        q.push(make_pair(node->right, layer + 1));
    }
    return result;
  }
};

int main() {
  Solution solution;
  vector<int> layerorder = {3, 9, 20, NULL_FLAG, NULL_FLAG, 15, 7};
  TreeNode *root = buildTree(layerorder);
  vector<vector<int>> result = solution.levelOrder(root);
  for (vector<int> result_item : result) {
    for (int i : result_item) {
      cout << i << " ";
    }
    cout << endl;
  }
}
