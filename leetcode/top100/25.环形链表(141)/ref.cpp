// TODO: 快慢指针
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
  bool hasCycle(ListNode *head) {
    // 移动快慢指针找环
    ListNode *slow = head, *fast = head;
    while (fast) {
      slow = slow->next;
      if (!fast->next) {
        // 奇数个节点的时候走这里，偶数个节点的时候while循环完成退出
        break;
      }
      fast = fast->next->next;
	  if(slow==fast){
		// 快慢指针相遇，有环
		return true;
	  }
    }
	return false;
  }
};

int main() {
  Solution solution;
  // 构造链表
  int pos = 1;
  vector<int> list = {3, 2, 0, -4}; // True
                                    //   int pos = -1;
                                    //   vector<int> list = {1}; // False
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
  bool result = solution.hasCycle(head);

  cout << result << endl;
}
