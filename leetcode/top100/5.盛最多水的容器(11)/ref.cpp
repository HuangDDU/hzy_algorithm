#include <iostream>
#include <vector>
using namespace std;

int maxArea(vector<int>& height) {
        int i=0, j=height.size()-1; // 左右指针对应两木板
        int result = 0;
        while(i < j){
            int tmp_result = (j-i)*min(height[i], height[j]);
            result = max(result, tmp_result);
            // 向中心移动短板
            if(height[i]<height[j]){
                i++;
            }else{
                j--;
            }
        }
        return result;
}

int main(){
    vector<int> height = {1,8,6,2,5,4,8,3,7};
    cout << maxArea(height) << endl;
}