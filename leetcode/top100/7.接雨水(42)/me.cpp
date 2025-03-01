// 对于每个位置寻找雨水深度
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

class Solution {
 public:
  int trap(vector<int> &height) {
    int result = 0;
    int max_layer = *max_element(height.begin(), height.end());
    for (int layer = 0; layer <= max_layer; layer++) {
      int tmp_result = 0; // 左右挡板之间的雨水量
      bool flag = false; // 是否出现了左挡板
      for (int i = 0; i < height.size(); i++) {
        int h = height[i];
        if (h > layer) {
          if (flag) {
            // 右挡板
            result += tmp_result;
            tmp_result = 0;
          } else {
            // 左挡板
            flag = true;
          }
        } else {
          if (flag) {
            // 出现了左挡板，可以积水了
            tmp_result++;
          }
        }
      }
    }
    return result;
  }
};

int main() {
  vector<int> height = {0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1}; // 6
  Solution s;
  cout << s.trap(height) << endl;
}
