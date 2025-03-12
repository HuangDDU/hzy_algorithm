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
  ListNode *mergeTwoLists(ListNode *list1, ListNode *list2) {
    // 1个链表为空，直接返回另一个链表
    if (!list1) {
      return list2;
    }
    if (!list2) {
      return list1;
    }

    // 初始化，p1,p2分别为list1、2的工作结点，head_p和p分别为目标链表头节点和工作结点
    ListNode *p1, *p2, *head_p, *p;
    if (list1->val <= list2->val) {
      head_p = p = list1;
      p1 = list1->next;
      p2 = list2;
    } else {
      head_p = p = list2;
      p1 = list1;
      p2 = list2->next;
    }

	// 主要过程，选择较小结点挂到目标链表尾部
    while (p1 && p2) {
      if (p1->val < p2->val) {
        p->next = p1;
        p = p->next;
        p1 = p1->next;
      } else {
        p->next = p2;
        p = p->next;
        p2 = p2->next;
      }
    }

	// 后处理，直接挂到尾部
    if (p1) {
      p->next = p1;
      
    }else{
      p->next = p2;
    }
	return head_p;
  }
};

int main() {
  Solution solution;

  vector<int> l1 = {1, 2, 4}, l2 = {1, 3, 4};
  ListNode *list1 = new ListNode(l1[0]), *list2 = new ListNode(l2[0]);
  ListNode *p = list1;
  for (int i = 1; i < l1.size(); i++) {
    ListNode *node = new ListNode(l1[i]);
    p->next = node;
    p = node;
  }
  p = list2;
  for (int i = 1; i < l2.size(); i++) {
    ListNode *node = new ListNode(l2[i]);
    p->next = node;
    p = node;
  }

  // 求解
  ListNode *result = solution.mergeTwoLists(list1, list2);
  p = result;
  while (p) {
    cout << p->val << ", ";
    p = p->next;
  }
}
