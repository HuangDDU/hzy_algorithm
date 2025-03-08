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
    if ((head == NULL) || (head->next == NULL)) {
      return head;
    } else {
      ListNode *next_node = head->next;
      ListNode *reversed_next = reverseList(next_node);
      next_node->next = head;
      head->next = NULL;
      return reversed_next;
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
