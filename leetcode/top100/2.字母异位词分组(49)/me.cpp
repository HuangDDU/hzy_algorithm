#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<algorithm>
using namespace std;


string getSortedStr(string str){
    // 返回字符串所有字母按字典序排序号的字符串
    string sortedStr(str);
    sort(sortedStr.begin(), sortedStr.end());
    return sortedStr;
}

vector<vector<string>> groupAnagrams(vector<string>& strs) {

    vector<vector<string>> result;
    map<string, vector<string>> sortedStrMap;

    for(vector<std::string>::iterator it=strs.begin(); it!=strs.end(); it++){
        // 暴力破解，遍历所有字符串
        string str = *it;
        string sortedStr = getSortedStr(str);
        map<string, vector<string>>::iterator pos = sortedStrMap.find(sortedStr);
        if(pos != sortedStrMap.end()){
            // 旧的则添加到现有的键值对
            pos->second.push_back(str);
        }else{
            // 新的则创建新的键值对
            sortedStrMap.insert(pair<string, vector<string>>(sortedStr, {str}));
        }
    }

    for(map<string, vector<string>>::iterator it=sortedStrMap.begin(); it!=sortedStrMap.end(); it++){
        result.push_back(it->second);
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