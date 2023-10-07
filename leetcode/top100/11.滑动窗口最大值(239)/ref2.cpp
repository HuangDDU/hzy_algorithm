// 使用双端队列代替优先级队列，每次插入元素之前清空比新元素小的值，减少元素存储个数
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

class Solution {
public:
  vector<int> maxSlidingWindow(vector<int> &nums, int k) {
    vector<int> res;
    // 初始化窗口，建双端队列存储下标，front对应的元素比back对应的元素都打
    deque<int> q;
    // 窗口滑动往堆中删除、添加元素
    for (int i = 0; i < nums.size() - k + 1; i++) {
      if (i == 0) {
        for (int j = 0; j < k; j++) {
          // 只保留比当前元素大的元素
          while (!q.empty() && nums[j] >= nums[q.back()]) {
            q.pop_back();
          }
          q.push_back(j);
        }
      } else {
        while (!q.empty() && nums[i + k - 1] >= nums[q.back()]) {
          q.pop_back();
        }
        q.push_back(i + k - 1);
        while (q.front() < i) {
          // 如果队列中最大元素不在窗口内，则删除
          q.pop_front();
        }
      }
      // 堆顶元素为最大值
      res.push_back(nums[q.front()]);
    }
    return res;
  }
};

int main() {
  vector<int> nums = {1, 3, -1, -3, 5, 3, 6, 7};
  int k = 3;
  // [3,3,5,5,6,7]
  // vector<int> nums = {-7, -8, 7, 5, 7, 1, 6, 0};
  // int k = 4;
  // [7,7,7,6,6]
  Solution solution;
  vector<int> res = solution.maxSlidingWindow(nums, k);
  for (int i : res) {
    cout << i << endl;
  }
}