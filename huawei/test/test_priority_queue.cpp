#include <iostream>
#include <queue>
#include <vector>
using namespace std;

struct Coordinate {
    int x;
    int y;
    int weight;

    Coordinate(int x, int y, int w) : x(x), y(y), weight(w) {}

    // 定义比较函数
    bool operator>(const Coordinate& other) const {
        return weight > other.weight;
    }
};

int main() {
    priority_queue<Coordinate, std::vector<Coordinate>, std::greater<Coordinate>> minHeap;

    // 添加坐标对
    minHeap.push(Coordinate(1, 2, 10));
    minHeap.push(Coordinate(3, 4, 5));
    minHeap.push(Coordinate(5, 6, 8));

    while (!minHeap.empty()) {
        Coordinate top = minHeap.top();
        minHeap.pop();
        std::cout << "Coordinate: (" << top.x << ", " << top.y << "), Weight: " << top.weight << std::endl;
    }

    return 0;
}