// 使用优先级队列，底层还是用堆
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

class Solution {
public:
  vector<int> maxSlidingWindow(vector<int> &nums, int k) {
    vector<int> res;
    // 初始化窗口，建优先级队列
    priority_queue<pair<int, int>> q;
    // 窗口滑动往堆中删除、添加元素
    for (int i = 0; i < nums.size() - k + 1; i++) {
      if (i == 0) {
        for(int j=0; j<k; j++){
          q.emplace(nums[j], j);
        }
      } else {
        q.emplace(nums[i+k-1], i+k-1); // 添加新纳入窗口的元素
        while(q.top().second < i){
          // 如果队列中最大元素不在窗口内，则删除
          q.pop(); 
        }
      }
      // 优先级队列头部元素为最大值
      res.push_back(q.top().first);
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