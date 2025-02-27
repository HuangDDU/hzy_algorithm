// 优化内层的两个循环，分别从左往右和从右往左扫。原本的
#include <algorithm>
#include <iostream>
#include <set>
#include <vector>
using namespace std;

vector<vector<int>> threeSum(vector<int> &nums) {
  vector<vector<int>> result;
  sort(nums.begin(), nums.end()); // 排序后方便去重
  for(int first=0; first< nums.size(); first++){
    // 固定好第一个值，后续两个指针为左右指针
    if(first>0 && nums[first] == nums[first-1]) continue; // 去重
    int third = nums.size()-1; // 右指针初始化
    for(int second = first+1; second<nums.size(); second++){
      if(second>first+1 && nums[second] == nums[second-1]) continue; // 去重
      while(second<third && nums[first]+nums[second]+nums[third]>0){
        third--;
      }
      if(second == third){
        break; // 退出条件
      }else if(nums[first]+nums[second]+nums[third] == 0){
        result.push_back({nums[first], nums[second], nums[third]}); // 找到了一个解
      }else{
        continue; // 当前左指针找不到解的，保持右指针不动，for循环控制左指针移动
      }
    }


  }
  return result;
}

int main() {
  // 1. 测试用例1
  // vector<int> nums = {-1,0,1,2,-1,-4};
  // [[-1,-1,2],[-1,0,1]]
  // 2. 执行时候的测试用例
  vector<int> nums = {3, 0, -2, -1, 1, 2};
  // [[-2,-1,3],[-2,0,2],[-1,0,1]]
  vector<vector<int>> res = threeSum(nums);
  // 这里简化输出
  for (vector<int> item : res) {
    for (int i : item) {
      cout << i << ",";
    }
    cout << endl;
  }
  return 0;
}
