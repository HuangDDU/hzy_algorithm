#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
  vector<int> dailyTemperatures(vector<int> &temperatures) {
    int n = temperatures.size();
    vector<int> result = vector<int>(n, 0);

    for (int i = 0; i < n; i++) {
      int flag = false;
      int j = i + 1;
      for (; j < n; j++) {
        if (temperatures[j] > temperatures[i]) {
          // 找到了距离最近的更高温度
          flag = true;
          break;
        }
      }
      if (flag) {
        result[i] = j - i;
      }
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
