#include <iostream>
#include <vector>
using namespace std;

class Provider {
public:
  int x, y;
  vector<vector<int>> target_vector; // TODO: 目标列表保存的方式
  Provider(int x, int y) {
    this->x = x;
    this->y = y;
  }
};

class Consumer {
public:
  int id;
  int x, y;
  int code_format; // 编码格式
  Consumer(int id, int x, int y, int code_format) {
    this->id = id;
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

class Controller {
public:
  vector<vector<int>> get_trajectory(Provider provider, Consumer consumer) {
    int start_x=provider.x, start_y=provider.y, end_x=consumer.x, end_y=consumer.y;
    vector<vector<int>> trajectory;
    trajectory.push_back({start_x, start_y}); // 起点
    if ((start_x != end_x) && (start_y != end_y)) {
      // 需要Transmitter
      trajectory.push_back({end_x, start_y}); // 先x轴方向移动后y轴方向移动
      // trajectory.push_back({start_x, end_y}); // 先y轴方向移动后x轴方向移动
    }
    trajectory.push_back({end_x, end_y}); // 终点
    return trajectory;
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
  // cout << "G_matrix OK" << endl;

  for (int i = 0; i < M; i++) {
    cin >> C_vector[i][0] >> C_vector[i][1] >> C_vector[i][2];
  }
  // cout << "C_vector OK" << endl;
  for (int i = 0; i < F; i++) {
    for (int j = 0; j < F; j++) {
      cin >> F_matrix[i][j];
    }
  }
  // cout << "F_matrix OK" << endl;

  // 对象构造
  Controller controller = Controller(); // 控制类，作为核心用作计算
  Provider provider = Provider(I, J);   // 单个Provider对象
  vector<Consumer> consumer_vector;     // Consumer列表
  vector<Transmitter> transmitter_vector; // Transmitter列表
  for (int i = 0; i < M; i++) {
    consumer_vector.push_back(
        Consumer(i+1, C_vector[i][0], C_vector[i][1], C_vector[i][2]));
  }

  // TODO: 核心部分，路径计算与保存
  for (int i = 0; i < M; i++) {
    Consumer consumer = consumer_vector[i];
    vector<vector<int>> trajectory = controller.get_trajectory(
        provider, consumer); // 计算从provider到consumer的路径
    if (trajectory.size() == 3) {
      // 传输轨迹发生了转弯，需要Transimtter
      int transmitter_x = trajectory[1][0], transmitter_y = trajectory[1][1]; // 轨迹上目标Transmitter的位置
      // 需要目标Transmitter的位置位置上是否有Transmitter存在了
      // TODO: 其实这里判断的有点简单了，应该判断从Provider到目标Transmitter的位置的路径上已经有Transmitter存在
      bool transmitter_exist_flag = false;
      int transmitter_id = -1;
      for (int j = 0; j < transmitter_vector.size(); j++) {
        Transmitter tmp_transmitter = transmitter_vector[j];
        if ((tmp_transmitter.x == transmitter_x) && (tmp_transmitter.y == transmitter_y)) {
          transmitter_exist_flag = true;
          transmitter_id = j;
          break;
        }
      }
      if (transmitter_exist_flag) {
        // 已经存在了Transmitter了，为其添加目标列表即可
        transmitter_vector[transmitter_id].target_vector.push_back({1, consumer.id, consumer.code_format});
      } else {
        // 不存在Transmitter，创建新的Transmitter
        Transmitter transmitter = Transmitter(transmitter_vector.size()+1,
                                              transmitter_x, transmitter_y);
        provider.target_vector.push_back({0, transmitter.id, consumer.code_format});
        transmitter.target_vector.push_back({1, consumer.id, consumer.code_format});
        transmitter_vector.push_back(transmitter);
      }
    } else {
      // 传输轨迹没有转弯，直达
      provider.target_vector.push_back({1, consumer.id, consumer.code_format});
    }
  }

  // 输出部分
  cout << transmitter_vector.size() << endl;
  // Provider输出
  vector<vector<int>> target_vector = provider.target_vector;
  cout << provider.x << " " << provider.y << " " << target_vector.size();
  for (int j = 0; j < target_vector.size(); j++) {
      cout << " " << target_vector[j][0] << " " << target_vector[j][1] << " " << target_vector[j][2];
  }
  cout << endl;
  // Transmitter输出
  for (int i = 0; i < transmitter_vector.size(); i++) {
    Transmitter transmitter = transmitter_vector[i];
    vector<vector<int>> target_vector = transmitter.target_vector; // 目标列表，每隔元素为一个三元组
    // i, j , d
    cout << transmitter.x << " " << transmitter.y << " " << target_vector.size();
    // 目标列表三元组输出
    for (int j = 0; j < target_vector.size(); j++) {
      cout << " " << target_vector[j][0] << " " << target_vector[j][1] << " " << target_vector[j][2];
    }
    cout << endl;
  }
  return 0;
}
