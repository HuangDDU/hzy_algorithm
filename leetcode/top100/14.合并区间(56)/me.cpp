// 布尔数组存储元素，最后扫描确定区间
#include <iostream>
#include <vector>
#include <set>
using namespace std;

class Solution {
public:
  vector<vector<int>> merge(vector<vector<int>> &intervals) {
    vector<vector<int>> res;
    set<int> empty_res; // 空区间
    // 布尔数组存储元素
    vector<int> bool_vector(10001, false);
    for (int i = 0; i < intervals.size(); i++) {
      int start = intervals[i][0],
          end = intervals[i][1] - 1; // 使用这样的左闭右开比较好
      if (end < start) {
        empty_res.insert(start);
        continue; // 空区间
      }
      for (int j = start; j <= end; j++) {
        bool_vector[j] = true;
      }
    }
    // 扫描确定区间
    int start = -1, end = -1;
    for (int i = 0; i < bool_vector.size(); i++) {
      if (!bool_vector[i]) {
        // 保存区间
        if (!(start == -1)) {
          res.push_back({start, end + 1}); // 左闭右开输出格式恢复
        }
        start = -1;
        end = -1;
      } else {
        // 延申或开辟新区间
        if (!(start == -1)) {
          // 延申区间
          end = i;
        } else {
          // 开辟区间
          start = i;
          end = i;
        }
      }
    }
    // 空区间填充
    for (int i : empty_res) {
      if (!bool_vector[i]) {
        // 不是区间，也不是区间尾部，单独成空区间
        if(i-1>=0){
          if((!bool_vector[i - 1])){
            res.push_back({i, i});
          }
        }else{
          res.push_back({i, i});
        }
      }                                                                                                         
    }
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