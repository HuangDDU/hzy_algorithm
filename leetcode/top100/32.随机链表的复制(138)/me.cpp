#include <iostream>
#include <map>
#include <vector>

using namespace std;

class Node {
public:
  int val;
  Node *next;
  Node *random;

  Node(int _val) {
    val = _val;
    next = NULL;
    random = NULL;
  }
};

class Solution {
public:
  Node *copyRandomList(Node *head) {
    // 第一次遍添加next指针
    Node *pre_head_new = new Node(0);
    Node *p = head, *p_new = pre_head_new;
    while (p) {
      p_new->next = new Node(p->val);
      p_new = p_new->next;
      p = p->next;
    }
    // 第二次遍历记录新旧结点的对应关系
    Node *head_new = pre_head_new->next;
    p = head, p_new = head_new;
    map<Node *, Node *> node_map;
    while (p) {
      node_map[p] = p_new;
      p = p->next;
      p_new = p_new->next;
    }

    // 第三次遍历实现random指针设置
    p = head, p_new = head_new;
    while (p) {
      p_new->random = node_map[p->random];
      p = p->next;
      p_new = p_new->next;
    }
    return pre_head_new->next;
  }
};

int main() {
  Solution solution;
  // 构造链表
  vector<vector<int>> list = {
      {7, -1}, {13, 0}, {11, 4}, {10, 2}, {1, 0}}; // 暂时用-1表示空结点
  Node *head = new Node(list[0][0]);
  Node *p = head;
  vector<Node *> node_ptr_list = {head};
  // 先添加next指针
  for (int i = 1; i < list.size(); i++) {
    Node *node_ptr = new Node(list[i][0]);
    node_ptr_list.push_back(node_ptr);
    p->next = node_ptr;
    p = node_ptr;
  }
  p->next = NULL;
  for (int i = 0; i < list.size(); i++) {
    if (list[i][1] >= 0) {
      node_ptr_list[i]->random = node_ptr_list[list[i][1]];
    }
  }

  Node *result = solution.copyRandomList(head);
  while (result) {
    cout << result->val;
    Node *random = result->random;
    if (random) {
      cout << "(random:" << random->val << ")";
    } else {
      cout << "(random:NULL)";
    }
    cout << endl;
    result = result->next;
  }
  cout << endl;
}
