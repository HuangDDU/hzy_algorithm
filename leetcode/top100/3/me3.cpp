// 这里出现了重大错误,使用表的过程中会重复使用打表中的元素.
// // 基本模板
// #include <algorithm>
// #include <iostream>
// #include <map>
// #include <set>
// #include <vector>
// using namespace std;

// vector<vector<int>> threeSum(vector<int> &nums) {
//   // 打表，空间换时间
//   // map<vector<int>, int> m1;         // 下标：差距值
//   map<int, vector<vector<int>>> m2; // 差距值：下标列表
//   for (int i = 0; i < nums.size() - 2; i++) {
//     for (int j = i + 1; j < nums.size() - 1; j++) {
//       int rest = -(nums[i] + nums[j]); // 避免多次加法计算，只运算一次
//       // m1[{i,j}] = rest;
//       auto pos = m2.find(rest);
//       if (pos != m2.end()) {
//         m2[rest].push_back({i, j});
//       } else {
//         m2[rest] = {{i, j}};
//       }
//     }
//   }
//   // 使用表
//   set<vector<int>> res_set; // 外部set，内部三元组使用vector，避免出现重复三元组
//   for (int k = 2; k < nums.size(); k++) {
//     auto pos = m2.find(nums[k]);
//     if (pos != m2.end()) {
//       for (vector<int> pair_item : m2[nums[k]]) {
//         int i = pair_item[0], j = pair_item[1];
//         vector<int> item = {nums[i], nums[j], nums[k]};
//         sort(item.begin(), item.end()); // 排序来避免重复
//         res_set.insert(item);
//         break; // 及时跳出最内层循环，避免重复添加
//       }
//     }
//   }
//   vector<vector<int>> res(res_set.begin(), res_set.end()); // 整体转化为vector
//   return res;
// }

// int main() {
//   // 1. 测试用例1
//   // vector<int> nums = {-1,0,1,2,-1,-4};
//   // [[-1,-1,2],[-1,0,1]]
//   // 2. 执行时候的测试用例
//   vector<int> nums = {3, 0, -2, -1, 1, 2};
//   // [[-2,-1,3],[-2,0,2],[-1,0,1]]
//   vector<vector<int>> res = threeSum(nums);
//   // 这里简化输出
//   for (vector<int> item : res) {
//     for (int i : item) {
//       cout << i << ",";
//     }
//     cout << endl;
//   }
//   return 0;
// }
