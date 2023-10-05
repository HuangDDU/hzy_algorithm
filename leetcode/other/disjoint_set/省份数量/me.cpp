// 连通分量个数计算
#include <iostream>
#include <vector>
using namespace std;

// 深度优先遍历计算计算连通分量的数目
void dfs(vector<vector<int>>& isConnected, vector<bool>& visited, int index){
    visited[index] = true;
    for(int i=0; i<isConnected.size(); i++){
        if((visited[i]==false)&&(isConnected[index][i]==1)){
            dfs(isConnected, visited, i);
        }
    }
}
int findCircleNum(vector<vector<int>>& isConnected) {
    int result = 0;
    vector<bool> visited(isConnected.size(), false); // 初始化遍历的记录数组
    for(int i=0; i<isConnected.size(); i++){
        // 遍历该连通分量
        if(visited[i]==false){
            dfs(isConnected, visited, i);
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