// NTOE: 看反了，没住注意到是逆序排列
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
    pair<int, ListNode *> result_pair = addTwoNumbersRecursion(list1, list2);
    int pre_carry_over = result_pair.first;
    ListNode *pre_p = result_pair.second;
    if (pre_carry_over) {
      ListNode *p = new ListNode(1);
      p->next = pre_p;
      return p;
    } else {
      return pre_p;
    }
  }
  pair<int, ListNode *> addTwoNumbersRecursion(ListNode *list1,
                                               ListNode *list2) {
    if (!list1) {
      // 递归终止条件
      return make_pair(0, nullptr);
    } else {
      // 递归计算低位链表和进位
      pair<int, ListNode *> result_pair =
          addTwoNumbersRecursion(list1->next, list2->next);
      int pre_carry_over = result_pair.first;
      ListNode *pre_p = result_pair.second;
      int value = list1->val + list2->val + pre_carry_over;

      // 当前位置构造新结点
      int now_carry_over = value / 10;
      value = value % 10;
      ListNode *p = new ListNode(value);
      p->next = pre_p;

      return make_pair(now_carry_over, p);
    }
  }
};

int main() {
  Solution solution;

  vector<int> l1 = {2, 4, 3}, l2 = {5, 6, 4}; // {7, 0, 8}
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
