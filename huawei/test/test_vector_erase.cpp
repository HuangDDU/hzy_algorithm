#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main(){
    vector<vector<int>> v = {{1,2,3}, {4,5,6}, {7,8,9}};
    auto it = find(v.begin(), v.end(), std::vector<int>{4,5,6});
    v.erase(it);
    for(int i=0; i<v.size(); i++){
        for(int j=0; j<v[0].size(); j++){
            cout << v[i][j] << " ";
        }
        cout << endl;
    }
}
