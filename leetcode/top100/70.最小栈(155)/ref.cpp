#include <iostream>
#include <map>
#include <stack>

using namespace std;

class MinStack {
public:
  stack<int> st;     // 普通栈
  stack<int> min_st; // 最小栈
  MinStack() {
    min_st.push(INT_MAX); // 最小栈栈底元素，方便push时的逻辑
  }

  void push(int val) {
    st.push(val);
    min_st.push(min(val, min_st.top()));
  }

  void pop() {
    st.pop();
    min_st.pop();
  }

  int top() { return st.top(); }

  int getMin() { return min_st.top(); }
};

int main() {
  MinStack *obj = new MinStack();
  obj->push(-2);
  obj->push(0);
  obj->push(-3);
  int result;
  result = obj->getMin(); // -3
  cout << result << ", ";
  obj->pop();
  result = obj->top(); // 0
  cout << result << ", ";
  result = obj->getMin(); // -2
  cout << result << ", ";
}
