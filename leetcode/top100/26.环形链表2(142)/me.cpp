#include <iostream>
#include <unordered_set>
#include <vector>

using namespace std;

struct ListNode {
  int val;
  ListNode *next;
  ListNode(int x) : val(x), next(NULL) {}
};

class Solution {
public:
  ListNode *detectCycle(ListNode *head) {
    unordered_set<ListNode *> value_set; // 保存访问过的节点指针
    ListNode *p = head;
    while (p) {
      if (value_set.find(p) == value_set.end()) {
        value_set.insert(p);
        p = p->next;
      } else {
        // 找到之前加入的结点指针
        return p;
      }
    }
    return NULL;
  }
};

int main() {
  Solution solution;
  // 构造链表
  //   int pos = 1;
  //   vector<int> list = {3, 2, 0, -4}; // 1
  int pos = -1;
  vector<int> list = {1}; // -1
  ListNode *head = new ListNode(list[0]);
  ListNode *p = head;
  for (int i = 1; i < list.size(); i++) {
    ListNode *node = new ListNode(list[i]);
    p->next = node;
    p = node;
  }
  p->next = NULL;
  // 添加环结点
  if (pos >= 0) {
    ListNode *loop_node = head;
    for (int i = 1; i <= pos; i++) {
      loop_node = loop_node->next;
    }
    p->next = loop_node;
  }

  // 求解
  ListNode *result = solution.detectCycle(head);

  cout << result << endl;
}
