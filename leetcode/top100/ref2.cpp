// TODO:
// 参考2，修改了原本链表，快慢指针（1步1次和2步1次）找中点，反转后半部分链表，开始比较
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
  bool isPalindrome(ListNode *head) {
    // 移动快慢指针找到中点
    ListNode *slow = head, *fast = head;
    while (fast) {
      slow = slow->next;
	  if(!fast->next){
		// 奇数个节点的时候走这里，偶数个节点的时候while循环完成推出
		break;
	  }
      fast = fast->next->next;
    }

    // 反转后半部分链表
    ListNode *end_half = reverseList(slow);

    // 对比前半部分和后半部分链表
    ListNode *start_half = head;
    while (end_half) {
      if (end_half->val != start_half->val) {
        return false;
      }
      end_half = end_half->next;
      start_half = start_half->next;
    }
    return true;
  }

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
  vector<int> list = {1, 2, 2, 1}; // True
  ListNode *head = new ListNode(list[0]);
  ListNode *p = head;
  for (int i = 1; i < list.size(); i++) {
    ListNode *node = new ListNode(list[i]);
    p->next = node;
    p = node;
  }
  p->next = NULL;
  // 求解
  bool result = solution.isPalindrome(head);

  cout << result << endl;
}
