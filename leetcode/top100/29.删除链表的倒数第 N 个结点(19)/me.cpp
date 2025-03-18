#include <iostream>
#include <vector>
using namespace std;

struct ListNode {
  int val;
  ListNode *next;
  ListNode(int x) : val(x), next(NULL) {}
};

class Solution {
public:
  ListNode *removeNthFromEnd(ListNode *head, int n) {
    ListNode *slow = head, *fast = head;
    // 快指针先走n步
    for (int i = 0; i < n; i++) {
      fast = fast->next;
    }
    if (fast) {
      fast = fast->next;
      // 快指针慢指针同时移动，直到快走指针到底
      while (fast) {
        slow = slow->next;
        fast = fast->next;
      }
      // 此时慢指针为待删除结点的前驱
      ListNode *p = slow->next;
      slow->next = p->next;
      delete p;

    } else {
      ListNode *p = head;
      head = head->next;
      delete p;
    }
    return head;
  }
};

int main() {
  Solution solution;
  // 构造链表
  vector<int> list = {1, 2, 3, 4, 5};
  int n = 2;
  ListNode *head = new ListNode(list[0]);
  ListNode *p = head;
  for (int i = 1; i < list.size(); i++) {
    ListNode *node = new ListNode(list[i]);
    p->next = node;
    p = node;
  }
  p->next = NULL;

  ListNode *result = solution.removeNthFromEnd(head, n);
  while (result) {
    cout << result->val << ",";
    result = result->next;
  }
  cout << endl;
}
