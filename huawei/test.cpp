#include <iostream>
#include <vector>
using namespace std;

class Provider {
public:
  int x, y;
  int target_vector; // TODO: 目标列表保存的方式
  Provider(int x, int y) {
    this->x = x;
    this->y = y;
  }
};

class Consumer {
public:
  int id;
  int x, y;
  int code_format; //编码格式
  Consumer(int id, int x, int y, int code_type) {
    this-> id = id;
    this->x = x;
    this->y = y;
    this->code_format = code_format;
  }
};

class Transmitter {
public:
  int id;
  int x, y;
  vector<vector<int>> target_vector; // TODO: 目标列表保存的方式
  Transmitter(int id, int x, int y) {
    this->id = id;
    this->x = x;
    this->y = y;
  }
};

// 5 3 2 10
// 2 3
// 20 15 10 10 10
// 10 10 10 20 10
// 10 15 10 90 10
// 10 20 10 10 10
// 10 10 10 10 10
// 0 0 0
// 3 1 0
// 1 3 0
// 0 10
// 20 0

int main() {
  // 输入部分
  int N, M, F, P;
  int I, J;  
  cin >> N >> M >> F >> P;
  vector<vector<int>> G_matrix(N, vector<int>(N)); // 地图矩阵，每个单元的传输代价
  vector<vector<int>> C_vector(M, vector<int>(3)); // Consumer三元组
  vector<vector<int>> F_matrix(F, vector<int>(F)); // 转换矩阵
  cin >> I >> J;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      cin >> G_matrix[i][j];
    }
  }
  cout << "G_matrix OK" << endl;

  for (int i = 0; i < M; i++) {
    cin >> C_vector[i][0] >> C_vector[i][1] >> C_vector[i][2];
  }
  cout << "C_vector OK" << endl;
  for (int i = 0; i < F; i++) {
    for (int j = 0; j < F; j++) {
      cin >> F_matrix[i][j];
    }
  }
  cout << "F_matrix OK" << endl;

  // 对象构造
  Provider provider = Provider(I, J); // 单个Provider对象
  vector<Consumer> consumer_vector;  // Consumer列表
  vector<Transmitter> transmitter_vector; // Transmitter列表
  for (int i = 0; i < M; i++) {
    consumer_vector.push_back(Consumer(i, C_vector[i][0], C_vector[i][1], C_vector[i][2]));
  }

  // TODO: 核心部分，路径计算与保存

  // 输出部分
  cout << transmitter_vector.size() << endl; 
  // TODO: Provider输出
  // Transmitter输出
  for (int i = 0; i < transmitter_vector.size(); i++) {
    Transmitter transmitter = transmitter_vector[i];
    vector<vector<int>> target_vector = transmitter.target_vector; // 目标列表，每隔元素为一个三元组
    // i, j , d
    cout << transmitter.x << " " << transmitter.y << " " << target_vector.size();
    // 目标列表三元组输出
    for(int i=0; i<target_vector.size(); i++){
      cout << " " << target_vector[i][0] << " " << target_vector[i][1] << " " << target_vector[i][2];
    }
    cout << endl;
  }
  return 0;
}
