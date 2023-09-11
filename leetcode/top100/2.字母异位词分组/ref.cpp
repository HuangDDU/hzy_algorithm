// 基本模板
#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<algorithm>
using namespace std;

vector<vector<string>> groupAnagrams(vector<string>& strs) {

    vector<vector<string>> result;
    map<string, vector<string>> mp;

    for(string str : strs){
        // 转化为字符数组后，简化排序操作
        string key = str;
        sort(key.begin(), key.end());
        mp[key].push_back(str); // 这里直接使用push_back,不用做键的判断
    }
    // 提取已有有序字符数组对应的字符串列表
    for (map<string, vector<string>>::iterator it = mp.begin(); it != mp.end(); ++it) {
        result.emplace_back(it->second);
    }
    return result;
}

int main(){
    vector<string> strs = {"eat","tea","tan","ate","nat","bat"};
    vector<vector<string>> result = groupAnagrams(strs);
    // 这里简化输出，
    for(vector<string> result_item : result){
        for(string str : result_item){
            cout<<str<<",";
        }
        cout<<endl;
    }
    return 0;
}
