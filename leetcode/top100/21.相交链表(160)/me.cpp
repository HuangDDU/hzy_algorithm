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
  ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
    ListNode *result = NULL;
    unordered_set<ListNode *> node_set; // 集合保存A链结点
    ListNode *p = headA;
    while(p){
      node_set.insert(p);
      p = p->next;
    }
    p = headB;
    while(p){
      if(node_set.find(p) != node_set.end()){
        // 找到了相交结点
        result = p;
        break;
      }
      p = p->next;
    }
    return result;
  }
};

int main() {
  Solution solution;
  vector<int> listA = {4, 1, 8, 4, 5}, listB = {5, 6, 1, 8, 4, 5};
  int skipA = 2, skipB = 3;
  ListNode *headA = new ListNode(listA[0]), *headB = new ListNode(listB[0]);
  // 先构造A链，记录相交结点
  ListNode *p = headA, *intersect_node = headA;
  for (int i = 1; i < listA.size(); i++) {
    ListNode *node = new ListNode(listA[i]);
    if (i == skipA) {
      // 相交结点
      intersect_node = node;
    }
    p->next = node;
    p = node;
  }
  // 构造B链到相交结点
  p = headB;
  for (int i = 1; i < skipB; i++) {
    ListNode *node = new ListNode(listB[i]);
    p->next = node;
    p = node;
  }
  p->next = intersect_node;
  cout << "预期相交结点：" << intersect_node << endl;
  cout << "预期相交结点值：" << intersect_node->val << endl;

  ListNode *result = solution.getIntersectionNode(headA, headB);
  cout << "实际相交结点：" << result << endl;
  cout << "实际相交结点值：" << result->val << endl;
}
