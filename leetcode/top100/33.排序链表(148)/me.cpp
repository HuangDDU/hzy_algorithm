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
  ListNode *sortList(ListNode *head) {
    if (!head || !head->next)
      return head; // 递归终止条件

    // 分
    ListNode *slow = head, *fast = head, *slow_pre = NULL; // 慢、快、慢前驱结点
    while (fast) {
      slow_pre = slow;
      slow = slow->next;
      fast = fast->next;
      if (fast)
        fast = fast->next;
    }
    slow_pre->next = NULL; // 断开链表

    // 治
    ListNode *head_sorted = sortList(head); // 递归
    ListNode *slow_sorted = sortList(slow); // 递归

    // 合
    ListNode *result = new ListNode(0); // 初始空结点
    ListNode *p = result;
    while (head_sorted && slow_sorted) { // 合并
      if (head_sorted->val < slow_sorted->val) {
        p->next = head_sorted;
        head_sorted = head_sorted->next;
      } else {
        p->next = slow_sorted;
        slow_sorted = slow_sorted->next;
      }
      p = p->next;
    }
    if (head_sorted) {
      p->next = head_sorted;
    } else {
      p->next = slow_sorted;
    }

    return result->next; // 返回合并后的链表
  }
};

int main() {
  Solution solution;
  // 构造链表
  //   vector<int> list = {4, 2, 1, 3};
  vector<int> list = {-1, 0, 3, 4, 5};
  ListNode *head = new ListNode(list[0]);
  ListNode *p = head;
  for (int i = 1; i < list.size(); i++) {
    ListNode *node = new ListNode(list[i]);
    p->next = node;
    p = node;
  }
  p->next = NULL;
  ListNode *result = solution.sortList(head);
  while (result) {
    cout << result->val << ",";
    result = result->next;
  }
  cout << endl;
}
