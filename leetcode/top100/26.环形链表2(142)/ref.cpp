// TODO: 快慢指针，慢指针入环之后，必然在一次圈内被快指针追上
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
  ListNode *detectCycle(ListNode *head) {
    // 移动快慢指针找环
    bool is_loop = false;
    ListNode *slow = head, *fast = head;
    while (fast) {
      slow = slow->next;
      if (!fast->next) {
        // 奇数个节点的时候走这里，偶数个节点的时候while循环完成退出
        break;
      }
      fast = fast->next->next;
      if (slow == fast) {
        // 快慢指针相遇，有环
        is_loop = true;
      }
    }

    if (is_loop) {
      // 有环则从起始点开始新的指针，与慢指针同步移动后必然相遇
      ListNode *new_p = head;
      while (new_p != slow) {
        new_p = new_p->next;
        slow = slow->next;
      }
      return new_p;
    } else {
      return NULL;
    }
  }
};

int main() {
  Solution solution;
  // 构造链表
  int pos = 1;
  vector<int> list = {3, 2, 0, -4}; // 1
                                    //   int pos = -1;
                                    //   vector<int> list = {1}; // -1
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
  ListNode *result = solution.detectCycle(head);

  cout << result << endl;
}
