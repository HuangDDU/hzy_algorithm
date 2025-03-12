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
  ListNode *addTwoNumbers(ListNode *list1, ListNode *list2) {
    ListNode *head_p = new ListNode(0); // 初始化简化，后续使用其next
    ListNode *p = head_p;

    int carry_over = 0; // 低位向高位置的进位
    // 两个链表对应位置相加
    while (list1 && list2) {

      int value = list1->val + list2->val + carry_over;
      carry_over = value / 10;
      p->next = new ListNode(value % 10);
      p = p->next;

      list1 = list1->next;
      list2 = list2->next;
    }

    // 剩余的单个链表，带着进位相加
    ListNode *last_p = list1 ? list1 : list2;
    while (last_p&&carry_over) {
      int value = last_p->val + carry_over;
      carry_over = value / 10;
      p->next = new ListNode(value % 10);
      p = p->next;
      last_p = last_p->next;
    }

	// 最后有进位表示要添加额外结点，没有进位则把单个链表的数字直接拉过来
	if(carry_over){
		p->next = new ListNode(1);
	}else{
		p->next = last_p;
	}

    return head_p->next;
  }
};

int main() {
  Solution solution;

//   vector<int> l1 = {2, 4, 3}, l2 = {5, 6, 4}; // {7, 0, 8}
  vector<int> l1 = {9, 9, 9, 9, 9, 9, 9},
              l2 = {9, 9, 9, 9}; // {8, 9, 9, 9, 0, 0, 0, 1}
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
  ListNode *result = solution.addTwoNumbers(list1, list2);
  p = result;
  while (p) {
    cout << p->val << ", ";
    p = p->next;
  }
}
