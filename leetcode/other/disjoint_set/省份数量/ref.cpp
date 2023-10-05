// 并查集计算
#include <iostream>
#include <vector>
using namespace std;

int find(vector<int> &parent, int index)
{
    if (parent[index] == index)
    {
        // 根节点
        return index;
    }
    else
    {
        // 非根结点，每次查询的时候修改，直接指向根节点，用作路径优化
        parent[index] = find(parent, parent[index]);
        return parent[index];
    }
    // return (parent[index] == index) ? index : (parent[index] = find(parent, parent[index]));;
}

void merge(vector<int> &parent, int source, int target)
{
    // 把source的集合添加到target内部
    parent[find(parent, source)] = find(parent, parent[target]);
}

int findCircleNum(vector<vector<int>> &isConnected)
{
    int n = isConnected.size();
    int result = 0;
    // 初始化，各个结点独立
    vector<int> parent(n);
    for (int i = 0; i < n; i++)
    {
        parent[i] = i;
    }
    // 遍历邻接矩阵，合并结点，注意这里的遍历右上角矩阵
    for (int i = 0; i < n; i++)
    {
        for (int j = i+1; j < n; j++)
        {
            if (isConnected[i][j] == 1)
            {
                merge(parent, j, i);
            }
        }
    }
    // 记录根节点个数
    for (int i = 0; i < n; i++)
    {
        if (parent[i] == i)
        {
            result++;
        }
    }
    return result;
}


int main()
{
    int n;
    cin >> n;
    vector<vector<int>> isConnected(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cin >> isConnected[i][j];
        }
    }
    int result = findCircleNum(isConnected);
    cout << result << endl;
    return 0;
}

// 3
// 1 1 0
// 1 1 0
// 0 0 1
// 解：2


// 15
// 1 0 0 0 0 0 0 0 0 1 0 0 0 0 0 
// 0 1 0 1 0 0 0 0 0 0 0 0 0 1 0 
// 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 
// 0 1 0 1 0 0 0 1 0 0 0 1 0 0 0 
// 0 0 0 0 1 0 0 0 0 0 0 0 1 0 0 
// 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 
// 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 
// 0 0 0 1 0 0 0 1 1 0 0 0 0 0 0 
// 0 0 0 0 0 0 0 1 1 0 0 0 0 0 0 
// 1 0 0 0 0 0 0 0 0 1 0 0 0 0 0 
// 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 
// 0 0 0 1 0 0 0 0 0 0 0 1 0 0 0 
// 0 0 0 0 1 0 0 0 0 0 0 0 1 0 0 
// 0 1 0 0 0 0 0 0 0 0 0 0 0 1 0
// 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
// 解：8

// 4
// 1 0 0 1 
// 0 1 1 0 
// 0 1 1 1 
// 1 0 1 1
// 解：1