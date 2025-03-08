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
  ListNode *reverseList(ListNode *head) {
    if (head == NULL) {
      return NULL;
    }
    vector<ListNode *> rlr = reverseListRecursion(head);
    return rlr[0];
  }
  vector<ListNode *> reverseListRecursion(ListNode *head) {
    if (head->next == NULL) {
      // 终止条件，单个节点
      return {head, head};
    } else {
      // 递归调用
      vector<ListNode *> rlr = reverseListRecursion(head->next);
      ListNode *reversed_head = rlr[0], *reversed_tail = rlr[1];
      reversed_tail->next = head;
      head->next = NULL;
      reversed_tail = head;
      return {reversed_head, reversed_tail}; // 分别返回反转后的头尾节点
    }
  }
};

int main() {
  Solution solution;
  // 构造链表
  vector<int> list = {1, 2, 3, 4, 5};
  ListNode *head = new ListNode(list[0]);
  ListNode *p = head;
  for (int i = 1; i < list.size(); i++) {
    ListNode *node = new ListNode(list[i]);
    p->next = node;
    p = node;
  }
  p->next = NULL;
  ListNode *result = solution.reverseList(head);
  while (result) {
    cout << result->val << ",";
    result = result->next;
  }
  cout << endl;
}
