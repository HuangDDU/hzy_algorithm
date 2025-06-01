#include <iostream>
#include <stack>
#include <vector>

using namespace std;

class Solution {
public:
  vector<int> dailyTemperatures(vector<int> &temperatures) {
    int n = temperatures.size();
    vector<int> result = vector<int>(n, 0);
    stack<pair<int, int>> st; // 栈中为未找到目标的元素下标及其值，为最小栈
    for (int i = 0; i < n; i++) {
      int value = temperatures[i];
      while (!(st.empty()) && (st.top().second < value)) {
        pair<int, int> p = st.top();
        int pre_i = p.first;
        result[pre_i] = i - pre_i;
        st.pop();
      }
      st.push(make_pair(i, value));
    }
    return result;
  }
};

int main() {
  Solution solution;
  vector<int> temperatures = {73, 74, 75, 71, 69, 72, 76, 73};
  vector<int> result = solution.dailyTemperatures(temperatures);
  for (int i : result) {
    cout << i << endl;
  }
}
