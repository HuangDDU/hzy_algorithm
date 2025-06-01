#include <iostream>
#include <map>
#include <stack>

using namespace std;

class MinStack {
public:
  stack<int> st;     // 普通栈
  stack<int> min_st; // 最小栈
  MinStack() {}

  void push(int val) {
    // 普通栈直接压入元素
    st.push(val);
    // 最小栈弹出栈顶元素到合适的位置
    stack<int> tmp_st;
    while ((!min_st.empty()) && (min_st.top() < val)) {
      tmp_st.push(min_st.top());
      min_st.pop();
    }
    // 置入新元素
    min_st.push(val);
    // 压入之前弹出的栈顶元素
    while (!tmp_st.empty()) {
      min_st.push(tmp_st.top());
      tmp_st.pop();
    }
  }

  void pop() {
    // 普通栈直接弹出元素并记录
    int val = st.top();
    st.pop();
    // 最小栈弹出对应元素之前元素到临时栈中
    stack<int> tmp_st;
    while (min_st.top() != val) {
      tmp_st.push(min_st.top());
      min_st.pop();
    }
    // 最小栈弹出对应元素
    min_st.pop();
    // 最小栈压入之前的元素
    while (!tmp_st.empty()) {
      min_st.push(tmp_st.top());
      tmp_st.pop();
    }
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
