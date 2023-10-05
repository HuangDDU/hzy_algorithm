#include <iostream>
#include <vector>
using namespace std;

int maxArea(vector<int>& height) {
    int result = 0;
    for(int i=0; i<height.size(); i++){
        for(int j=height.size()-1; j>i; j--){
            // 从两边往中间试
            int tmp_result = (j-i)*min(height[i], height[j]);
            if(tmp_result>result){
                result = tmp_result;
            }
        }
    }
    return result;
}

int main(){
    vector<int> height = {1,8,6,2,5,4,8,3,7};
    cout << maxArea(height) << endl;

}