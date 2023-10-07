// 使用堆存储滑动窗口里所有的元素值，但是堆的删除比较难搞，且堆中不允许有重复元素。
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
  vector<int> maxSlidingWindow(vector<int> &nums, int k) {
    vector<int> res;
    // 初始化窗口，建大顶堆
    vector<int> heap(nums.begin(), nums.begin() + k);
    // 窗口滑动往堆中删除、添加元素
    for (int i = 0; i < nums.size() - k + 1; i++) {
      if (i == 0) {
        make_heap(heap.begin(), heap.end());
      } else {
        // 删除窗口前的元素
        auto new_end = remove(heap.begin(), heap.end(), nums[i - 1]);
        heap.erase(new_end, heap.end());
        make_heap(heap.begin(), heap.end());
        // 添加窗口后的元素
        heap.push_back(nums[i + k - 1]);
        push_heap(heap.begin(), heap.end());
      }
      // 堆顶元素为最大值
      res.push_back(heap[0]);
    }
    return res;
  }
};

int main() {
  // vector<int> nums = {1, 3, -1, -3, 5, 3, 6, 7};
  // int k = 3;
  // [3,3,5,5,6,7]
  vector<int> nums = {-7, -8, 7, 5, 7, 1, 6, 0};
  int k = 4;
  // [7,7,7,6,6]
  Solution solution;
  vector<int> res = solution.maxSlidingWindow(nums, k);
  for (int i : res) {
    cout << i << endl;
  }
}