#include <iostream>
#include <map>
#include <vector>

int main() {
    std::map<std::string, std::vector<int>> order_map;

    // 添加值
    order_map["key1"] = {1, 2, 3};
    order_map["key2"] = {4, 5};

    // 输出值
    for (const auto& pair : order_map) {
        const std::string& key = pair.first;
        const std::vector<int>& values = pair.second;

        std::cout << "Key: " << key << ", Values: ";
        for (const auto& value : values) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}