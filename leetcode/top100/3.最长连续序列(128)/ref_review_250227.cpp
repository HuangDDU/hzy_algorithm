#include <iostream>
#include <set>
#include <vector>
using namespace std;

int longestConsecutive(vector<int> &nums) {
  if (nums.size() == 0){
    return 0;
  }
  int result = 1;
  set<int> nums_set(nums.begin(), nums.end()); // 打表记录
  for(int num: nums_set){
    int  tmp_num= num;
    int tmp_result = 1;
    if (nums_set.find(tmp_num-1) != nums_set.end()){
      // 当前值不是所在连续序列的最小值，不用做计数
      continue;
    }else{
      // 当前值时所在连续序列的最小值，累加计数
      while(nums_set.find(++tmp_num)!=nums_set.end()){
        // 不断尝试+1
        tmp_result += 1;
      }
      result = max(result, tmp_result);
    }
  }
  return result;
}

int main() {
  // vector<int> nums = {100, 4, 200, 1, 3, 2}; // 4
  vector<int> nums = {0,3,7,2,5,8,4,6,0,1}; // 9

  cout << longestConsecutive(nums) << endl;
}