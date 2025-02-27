#include <iostream>
#include <vector>
using namespace std;

int maxArea(vector<int> &height) {
  int max_area = 0;
  int i = 0, j = height.size() - 1;
  while (i < j) {
    int h = min(height[i], height[j]);
    max_area = max(max_area, h * (j - i));
    // 移动高度较小的指针，直到找到一个更大有效高度的位置。
    // 如果移动高度较大的指针，有效高度不会变大，宽度减小，面积一定减小。
    // 着这种移动移动指针的策略，有效的过滤掉了一些无效的情况。
    while (height[i] <= h && i < j) i++;
    while (height[j] <= h && i < j) j--;
  }
  return max_area;
}

int main() {
  vector<int> height = {1, 8, 6, 2, 5, 4, 8, 3, 7};
  cout << maxArea(height) << endl;
}