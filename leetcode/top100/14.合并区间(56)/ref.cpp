// 区间排序后扫描
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Solution {
public:
  vector<vector<int>> merge(vector<vector<int>> &intervals) {
    vector<vector<int>> res;
    sort(intervals.begin(), intervals.end());
    int l=intervals[0][0], r=intervals[0][1]; // 初始化区间
    for(int i=1; i<intervals.size(); i++){
      int tmp_l = intervals[i][0], tmp_r=intervals[i][1]; // 排序后确保l <= tmp_l
      if(tmp_l<=r){
        r = max(tmp_r, r); // 合并区间
      }else{
        res.push_back({l, r}); // 保存区间，并开辟新区见
        l = tmp_l;
        r = tmp_r;
      }
    }
    res.push_back({l, r}); // 保留最后的区间
    return res;
  }
};

int main() {
  vector<vector<int>> intervals = {{1, 3}, {2, 6}, {8, 10}, {15, 18}};
  // [[1,6],[8,10],[15,18]]
  // vector<vector<int>> intervals = {{1, 4}, {4, 5}};
  // [[1,5]]
  // vector<vector<int>> intervals = {{1, 4}, {5, 6}};
  // [[1,4],[5,6]]
  // vector<vector<int>> intervals = {{1, 4}, {0, 0}};
  // [[1,4]]
  // vector<vector<int>> intervals = {{5,7},{4,5},{3,5},{4,4},{0,1},{2,2},{5,6},{2,4},{5,6},{2,4},{1,1}};
  // [[0,1],[2,7]]
  Solution solution;
  vector<vector<int>> res = solution.merge(intervals);
  for (vector<int> item : res) {
    for (int i : item) {
      cout << i << " ";
    }
    cout << endl;
  }
}