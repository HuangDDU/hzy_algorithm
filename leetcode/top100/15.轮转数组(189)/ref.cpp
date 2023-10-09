// 使用一个临时变量循环赋值，这里循环次数用最小公倍数计算确定
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
  void rotate(vector<int> &nums, int k) {
    int n = nums.size();
    k = k % n;               // 避免周期性旋转
    int count = __gcd(k, n); // 循环次数到最小公倍数
    for (int start = 0; start < count; start++) {
      int current = start;
      int prev = nums[start];
      do {
        int next = (current + k) % n;
        swap(nums[next], prev);
        current = next;
      } while (start != current);
      // 循环到初始位置结束
    }
  }
};

int main() {
  vector<int> nums = {1, 2, 3, 4, 5, 6, 7};
  int k = 3;
  // [5,6,7,1,2,3,4]
  // vector<int> nums = {-1, -100, 3, 99};
  // int k = 2;
  // [3,99,-1,-100]
  Solution solution;
  solution.rotate(nums, k);
  for (int i : nums) {
    cout << i << " " << endl;
  }
}