#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int longestConsecutive(vector<int>& nums) {
    if (nums.size()==0) return 0;
    int min_value = *min_element(nums.begin(), nums.end());
    int max_value = *max_element(nums.begin(), nums.end());
    vector<bool> v(max_value-min_value+1, false); // 布尔向量的第i个位置表示,是否有min大i的元素
    // 对应位置元素写入布尔向量
    for(int i=0; i<nums.size(); i++){
        v[nums[i]-min_value] = true;
    }
    // 遍历布尔向量,计算最长元素
    int result = 0;
    int tmp = 0; 
    for(int i=0; i<v.size(); i++){
        if(v[i]){
            tmp += 1;
        }else{
            tmp = 0;
        }
        result = max(result, tmp);
    }
    return result;
}

int main() {
  vector<int> nums = {100,4,200,1,3,2};
  cout << longestConsecutive(nums) << endl;
}