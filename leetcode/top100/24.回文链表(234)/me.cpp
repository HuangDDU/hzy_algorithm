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
    // 遍历元素移动到数组
    vector<int> array;
    ListNode *p = head;
    while (p) {
      array.push_back(p->val);
      p = p->next;
    }
    // 双指针移动判断
    int left = 0, right = array.size() - 1;
    while (left < right) {
      if (array[left] != array[right]) {
        // 有元素不相等即判定为False
        return false;
      }
      left++;
      right--;
    }
    return true;
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
