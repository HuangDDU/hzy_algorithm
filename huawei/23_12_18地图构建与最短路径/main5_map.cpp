#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

class Provider {
public:
  int x, y;
  int left_transmitter_id = -1,
      right_transmitter_id = -1;     // 方便Provider所在行的路径合并
  vector<vector<int>> target_vector; // 目标列表保存的方式
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
  vector<vector<int>> target_vector; // 目标列表保存的方式
  Transmitter(int id, int x, int y) {
    this->id = id;
    this->x = x;
    this->y = y;
  }
};

// 每个单元格就是一个结点
enum direction {
  UP,
  RIGHT,
  DOWN,
  LEFT
}; // 动态规划的方向
class Node {
public:
  int x, y;
  int weight;
  direction best_direction;
  int distance;
  bool visited; // 在距离的动态规划是否访问过
  Node(){}
  Node(int x, int y, int weight) {
    this->x = x;
    this->y = y;
    this->weight = weight;
    this->distance = -1;
    this->visited = false;
  }
};

// 地图类
class Map {
public:
  int N;
  vector<vector<Node>> node_matrix;
  Map(vector<vector<int>> G_matrix) {
    this->N = G_matrix.size();
    for (int i = 0; i < this->N; i++) {
      vector<Node> tmp_v(this->N);
      for (int j = 0; j < this->N; j++) {
        tmp_v[j] = Node(i, j, G_matrix[i][j]);
      }
      node_matrix.push_back(tmp_v);
    }
  }
};

// 核心控制类
class Controller {
public:
  Controller(Provider provider, Map map) {
    // 地图上provider所在位置distance设置为0
    Node provider_node = map.node_matrix[provider.x][provider.y];
    provider_node.visited = true;
    provider_node.distance = 0;
    // provider所在的十字区域distance设置
    for (int i = provider.x - 1; i >= 0; i--) {
      // 所在列向上
      map.node_matrix[i][provider.y].visited = true;
      map.node_matrix[i][provider.y].best_direction = DOWN;
      map.node_matrix[i][provider.y].distance =
          map.node_matrix[i + 1][provider.y].distance +
          map.node_matrix[i][provider.y].weight;
    }
    for (int i = provider.x + 1; i < map.N; i++) {
      // 所在列向下
      map.node_matrix[i][provider.y].visited = true;
      map.node_matrix[i][provider.y].best_direction = UP;
      map.node_matrix[i][provider.y].distance =
          map.node_matrix[i - 1][provider.y].distance +
          map.node_matrix[i][provider.y].weight;
    }
    for (int i = provider.y - 1; i >= 0; i--) {
      // 所在行向左
      map.node_matrix[provider.x][i].visited = true;
      map.node_matrix[provider.x][i].best_direction = RIGHT;
      map.node_matrix[provider.x][i].distance =
          map.node_matrix[provider.x][i + 1].distance +
          map.node_matrix[provider.x][i].weight;
    }
    for (int i = provider.y + 1; i < map.N; i++) {
      // 所在行向右
      map.node_matrix[provider.x][i].visited = true;
      map.node_matrix[provider.x][i].best_direction = LEFT;
      map.node_matrix[provider.x][i].distance =
          map.node_matrix[provider.x][i - 1].distance +
          map.node_matrix[provider.x][i].weight;
    }
  }

  int get_distance(Map map, Provider provider, int x, int y) {
    //
    Node node = map.node_matrix[x][y];
    if (node.visited) {
      // 直接访问
      return node.distance;
    } else {
      // 根据坐标确定动态规划的方向
      if ((x < provider.x) && (y < provider.y)) {
        // 当前结点在provider的左上角
        int down_distance = get_distance(map, provider, x + 1, y);
        int right_distance = get_distance(map, provider, x, y + 1);
        if (down_distance <= right_distance) {
          node.best_direction = DOWN;
          node.distance = down_distance + node.weight;
        } else {
          node.best_direction = RIGHT;
          node.distance = right_distance + node.weight;
        }
      } else if ((x > provider.x)&&(y < provider.y)) {
          // 当前结点在provider的左下角
          int up_distance = get_distance(map, provider, x + 1, y);
          int right_distance = get_distance(map, provider, x, y + 1);
          if (up_distance <= right_distance) {
            node.best_direction = UP;
            node.distance = up_distance + node.weight;
          } else {
            node.best_direction = RIGHT;
            node.distance = right_distance + node.weight;
          }
        }
      else if ((x < provider.x)&&(y > provider.y)) {
          // 当前结点在provider的右上角
          int down_distance = get_distance(map, provider, x + 1, y);
          int left_distance = get_distance(map, provider, x, y - 1);
          if (down_distance <= left_distance) {
            node.best_direction = DOWN;
            node.distance = down_distance + node.weight;
          } else {
            node.best_direction = LEFT;
            node.distance = left_distance + node.weight;
          }
        }
      else {
        // 当前结点在provider的右下角
        int up_distance = get_distance(map, provider, x + 1, y);
        int left_distance = get_distance(map, provider, x, y - 1);
        if (up_distance <= left_distance) {
          node.best_direction = UP;
          node.distance = up_distance + node.weight;
        } else {
          node.best_direction = LEFT;
          node.distance = left_distance + node.weight;
        }
      }
      node.visited = true;
    }
    return node.distance;
  }

  void get_all_trajectory(Map map, Provider provider, vector<Consumer> consumer_vector) {
    for (int i = 0; i < consumer_vector.size(); i++) {
      Consumer consumer = consumer_vector[i];
      int distance = get_distance(map, provider, consumer.x, consumer.y);
      cout << consumer.id << " " << distance;
    }
  }
};

// 原始输入
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

// 当前方案输出
// 2
// 2 3 3 0 1 0 0 2 0 1 3 0
// 2 0 1 1 1 0
// 2 1 1 1 2 0

int main() {
  // 输入部分
  int N, M, F, P;
  int I, J;
  cin >> N >> M >> F >> P;
  vector<vector<int>> G_matrix(N,
                               vector<int>(N)); // 地图矩阵，每个单元的传输代价
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
  Provider provider = Provider(I, J);     // 单个Provider对象
  vector<Consumer> consumer_vector;       // Consumer列表
  vector<Transmitter> transmitter_vector; // Transmitter列表
  for (int i = 0; i < M; i++) {
    consumer_vector.push_back(
        Consumer(i + 1, C_vector[i][0], C_vector[i][1], C_vector[i][2]));
  }
  Map map = Map(G_matrix); // 地图map对象
  Controller controller = Controller(provider, map); // 控制类，作为核心用作计算

  // TODO: 核心部分，路径计算与保存
  // 动态规划计算轨迹
  controller.get_all_trajectory(map, provider, consumer_vector);
  // TODO: 轨迹回溯
  // TODO: 正向构建树结构
  // TODO: 树结构上DFS/BFS轨迹合并，并在拐弯处设置Tranmitter

  //   // 输出部分
  //   cout << transmitter_vector.size() << endl;
  //   // Provider输出
  //   vector<vector<int>> target_vector = provider.target_vector;
  //   cout << provider.x << " " << provider.y << " " << target_vector.size();
  //   for (int j = 0; j < target_vector.size(); j++) {
  //     cout << " " << target_vector[j][0] << " " << target_vector[j][1] << " "
  //          << target_vector[j][2];
  //   }
  //   cout << endl;
  //   // Transmitter输出
  //   for (int i = 0; i < transmitter_vector.size(); i++) {
  //     Transmitter transmitter = transmitter_vector[i];
  //     vector<vector<int>> target_vector =
  //         transmitter.target_vector; // 目标列表，每隔元素为一个三元组
  //     // i, j , d
  //     cout << transmitter.x << " " << transmitter.y << " "
  //          << target_vector.size();
  //     // 目标列表三元组输出
  //     for (int j = 0; j < target_vector.size(); j++) {
  //       cout << " " << target_vector[j][0] << " " << target_vector[j][1] << "
  //       "
  //            << target_vector[j][2];
  //     }
  //     cout << endl;
  //   }
  return 0;
}
