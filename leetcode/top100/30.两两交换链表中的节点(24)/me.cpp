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
  ListNode *swapPairs(ListNode *head) {
	if((head==nullptr)||(head->next==nullptr)){
		return head;
	}
    ListNode *p1 = head, *p2 = head->next; // 待交换的两个结点
    head = new ListNode(-1); // 新的虚拟头节点
	ListNode *pre = head; // 已经交换好的链表
    while ((p1 != nullptr) && (p2 != nullptr)) {
      // 交换结点
      p1->next = p2->next;
      p2->next = p1;
	  pre->next = p2;

	  pre = p1;
      p1 = p1->next;
      p2 = p1 ? p1->next : nullptr;
    }
    return head->next;
  }
};

int main() {
  Solution solution;
  // 构造链表
  vector<int> list = {1, 2, 3, 4};
  ListNode *head = new ListNode(list[0]);
  ListNode *p = head;
  for (int i = 1; i < list.size(); i++) {
    ListNode *node = new ListNode(list[i]);
    p->next = node;
    p = node;
  }
  p->next = NULL;

  ListNode *result = solution.swapPairs(head);
  while (result) {
    cout << result->val << ",";
    result = result->next;
  }
  cout << endl;
}
