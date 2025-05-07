#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
  bool canFinish(int numCourses, vector<vector<int>> &prerequisites) {
    // 本质是拓扑排序
    vector<int> indegree_vector = vector<int>(numCourses, 0);      // 入度vector
    vector<bool> visited_vector = vector<bool>(numCourses, false); // 是否访问过
    int visited_num = 0;
    vector<vector<int>> adj_vector_vector =
        vector<vector<int>>(numCourses, vector<int>()); // 相当于邻接表
    for (vector<int> prerequisity : prerequisites) {
      int from = prerequisity[0], to = prerequisity[1];
      indegree_vector[to]++;
      adj_vector_vector[from].push_back(to);
    }
    while (visited_num < numCourses) {
      // 找到一个入度为0的结点，找不到则跳出
      int available_index = -1;
      for (int i = 0; i < numCourses; i++) {
        if (!visited_vector[i] && indegree_vector[i] == 0) {
          available_index = i;
          break;
        }
      }
      if (available_index == -1)
        break;
      // 根据邻接表调整
      visited_num++;
      visited_vector[available_index] = true;
      for (int to : adj_vector_vector[available_index]) {
        indegree_vector[to]--;
      }
    }
    if (visited_num < numCourses) {
      return false;
    } else {
      return true;
    }
  }
};
int main() {
  Solution solution;
  //   int numCourses = 2;
  //   vector<vector<int>> prerequisites = {{1, 0}}; // true
  int numCourses = 2;
  vector<vector<int>> prerequisites = {{1, 0}, {0, 1}}; // false
  int result = solution.canFinish(numCourses, prerequisites);
  cout << result << endl;
}
