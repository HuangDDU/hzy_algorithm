#include<iostream>
#include<vector>
using namespace std;

class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        vector<int> result;
        // 二层暴力
        for(int i=0; i<nums.size(); i++){
            for(int j=i+1; j<nums.size();j++){
                if (nums[i]+nums[j] == target){
                    // 找到对应的解即返回
                    result.push_back(i);
                    result.push_back(j);
                    return result;
                }
            }
        }
        return result;
    }

    int main(){
        // 输入
        vector<int> nums = {2, 7, 11, 15};
        int target = 9;
        // 计算
        vector<int> result = twoSum(nums, target);
        // 输出
        for (vector<int>::iterator it = result.begin(); it != result.end(); it++) {
		    cout << *it << " ";
	    }
        return 0;
    }
};