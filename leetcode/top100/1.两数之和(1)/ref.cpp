#include<iostream>
#include<vector>
#include<unordered_map>
using namespace std;

vector<int> twoSum(vector<int>& nums, int target) {
    unordered_map<int, int> hashtable; // 元素值与元素下标的映射表，这里的技巧是以空间换时间
    for(int i=0; i<nums.size(); i++){
        unordered_map<int, int>::iterator it = hashtable.find(target-nums[i]);
        if(it != hashtable.end()){
            // 找到了与当前元素互补的元素，则得到结果返回当前下标与互补元素的下标
            return {it->second, i};
        }else{
            // 没有则先放入映射表中
            hashtable[nums[i]] = i;
        }
    }
    return {};
}

int main()
{
    // 输入
    vector<int> nums = {2, 7, 11, 15};
    int target = 9;
    // 计算
    vector<int> result = twoSum(nums, target);
    // 输出
    for(int i=0; i<result.size(); i++){
        cout<<result[i]<<" ";
    }
    return 0;
}