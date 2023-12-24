#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

class ProviderOrTranmitter{
public:
  int x, y;
  vector<vector<int>> target_vector; // 目标列表保存的方式
};


class Provider : public ProviderOrTranmitter {
public:
  Provider(int x, int y) {
    this->x = x;
    this->y = y;
  }
};


class Transmitter : public ProviderOrTranmitter{
public:
  int id;
  Transmitter(int id, int x, int y) {
    this->id = id;
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


// 每个单元格就是一个结点
enum direction { UP, RIGHT, DOWN, LEFT, UNKOWN}; // 动态规划的方向 上， 右， 下， 左， 未知
enum type { PROVIDER, CONSUMER, TRANSMITTER, EMPTY}; // 结点类型, Provider, Consumer, Transmitter, EMPTY
class Node {
public:
  int x, y;
  int weight;
  type node_type; // 结点类型
  int type_id; // 所在类型中的id号
  direction best_direction; // 动态规划的方向
  int distance; // 动态规划的最短距离
  bool visited; // 在距离的动态规划是否访问过
  vector<vector<int>> child_xy_list; // 孩子结点的xy位置列表
  Node() {}
  Node(int x, int y, int weight) {
    this->x = x;
    this->y = y;
    this->weight = weight;
    this->node_type = EMPTY;
    this->best_direction = UNKOWN;
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
  Controller(Provider &provider, Map &map) {
    // 地图上provider所在位置distance设置为0
    Node &provider_node = map.node_matrix[provider.x][provider.y];
    provider_node.visited = true;
    provider_node.distance = 0;
    // map.node_matrix[provider.x][provider.y].visited = true;
    // map.node_matrix[provider.x][provider.y].distance = 0;
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

  // 递归计算距离
  // TODO: 路径上可能有些结点是Consumer，不能穿透
  int get_distance(Map &map, Provider &provider, int x, int y) {
    //
    Node &node = map.node_matrix[x][y];
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
      } else if ((x > provider.x) && (y < provider.y)) {
        // 当前结点在provider的左下角
        int up_distance = get_distance(map, provider, x - 1, y);
        int right_distance = get_distance(map, provider, x, y + 1);
        if (up_distance <= right_distance) {
          node.best_direction = UP;
          node.distance = up_distance + node.weight;
        } else {
          node.best_direction = RIGHT;
          node.distance = right_distance + node.weight;
        }
      } else if ((x < provider.x) && (y > provider.y)) {
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
      } else {
        // 当前结点在provider的右下角
        int up_distance = get_distance(map, provider, x - 1, y);
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

  // 获得所有所有Consumer到Provier的最短路径
  // 预期输出
  // =======================================
  // 1 60
  // 2 40
  // 3 20
  void get_all_trajectory(Map &map, Provider &provider, vector<Consumer> consumer_vector) {
    // cout << "=======================================" << endl;
    for (int i = 0; i < consumer_vector.size(); i++) {
      Consumer consumer = consumer_vector[i];
      int distance = get_distance(map, provider, consumer.x, consumer.y);
      // cout << consumer.id << " " << distance << endl;
    }
  }

  // 递归回溯，找父结点构建轨迹
  void backwad_trajectory(Map &map, Provider &provider, int x, int y) {
    if (((x == provider.x) && (y == provider.y)) ||
        (map.node_matrix[x][y].child_xy_list.size() > 0)) {
      // 回溯到provider或已经回溯过了，结束停止
      return;
    } else {
      // 构建邻居并继续回溯
      Node &node = map.node_matrix[x][y];
      // Node &father_node = map.node_matrix[x][y]; // 必须要赋初值先就这样吧
      // if (node.best_direction == UP) {
      //   &father_node = map.node_matrix[x - 1][y];
      // } else if (node.best_direction == RIGHT) {
      //   &father_node = map.node_matrix[x][y + 1];
      // } else if (node.best_direction == DOWN) {
      //   &father_node = map.node_matrix[x + 1][y];
      // } else {
      //   &father_node = map.node_matrix[x][y - 1];
      // }
      // 需要这样引用传参
      int father_x, father_y;
      if (node.best_direction == UP) {
        father_x = x-1, father_y = y;
      } else if (node.best_direction == RIGHT) {
        father_x = x, father_y = y+1;
      } else if (node.best_direction == DOWN) {
        father_x = x+1, father_y = y;
      } else {
        father_x = x, father_y = y-1;
      }
      Node& father_node = map.node_matrix[father_x][father_y];
      backwad_trajectory(map, provider, father_node.x, father_node.y);
      father_node.child_xy_list.push_back({x, y});
    }
  }

  // 从所有Consumer作为叶子结点回溯到根节点
  void backward_all_trajectory(Map &map, Provider &provider, vector<Consumer> consumer_vector) {
    for (int i = 0; i < consumer_vector.size(); i++) {
      Consumer consumer = consumer_vector[i];
      backwad_trajectory(map, provider, consumer.x, consumer.y);
    }
  }

  // 递归从特定坐标开始DFS
  // TODO: DFS中的数据格式可能还是有点问题
  void dfs(Map &map, int x, int y, ProviderOrTranmitter &last, vector<Consumer> consumer_vector, vector<Transmitter>& transmitter_vector){
      Node& node = map.node_matrix[x][y];
      if(node.child_xy_list.size()>=2){
        // 分支点必然拐弯
        Transmitter transmitter = Transmitter(transmitter_vector.size()+1, node.x, node.y);
        last.target_vector.push_back({0, transmitter.id, 0}); // 暂时transmitter之间的传输都是0格式的
        transmitter_vector.push_back(transmitter); // vector不支持引用类型的对象，是深拷贝。暂时先占个位置
        // 遍历所有孩子
        for(int k=0; k<node.child_xy_list.size(); k++){
          vector<int> child_xy = node.child_xy_list[k];
          int child_x = child_xy[0], child_y = child_xy[1];
          dfs(map, child_x, child_y, transmitter, consumer_vector, transmitter_vector); // 用新的Transmitter作为消息转发
        }
        transmitter_vector[transmitter.id-1] = transmitter; // NOTE: 要放在后面，修改之后重新赋值
      }else if(node.child_xy_list.size()==1){
        int child_x = node.child_xy_list[0][0], child_y = node.child_xy_list[0][1];
        if(
          // NOTE: 这怎么能跑的？node.y写错了呢，等会重新提交看看效果
          // ((node.best_direction==UP)&&(node.x-child_x==-1)&&(node.x-child_y==0))||
          // ((node.best_direction==RIGHT)&&(node.x-child_x==0)&&(node.x-child_y==1))||
          // ((node.best_direction==DOWN)&&(node.x-child_x==1)&&(node.x-child_y==0))||
          // ((node.best_direction==LEFT)&&(node.x-child_x==0)&&(node.x-child_y==-1))
          ((node.best_direction==UP)&&(node.x-child_x==-1)&&(node.y-child_y==0))||
          ((node.best_direction==RIGHT)&&(node.x-child_x==0)&&(node.y-child_y==1))||
          ((node.best_direction==DOWN)&&(node.x-child_x==1)&&(node.y-child_y==0))||
          ((node.best_direction==LEFT)&&(node.x-child_x==0)&&(node.y-child_y==-1))
        ){
          // 没有拐弯
          dfs(map, child_x, child_y, last, consumer_vector, transmitter_vector);
        }else{
          // 拐弯了
          Transmitter transmitter = Transmitter(transmitter_vector.size()+1, node.x, node.y);
          last.target_vector.push_back({0, transmitter.id, 0}); // 暂时transmitter之间的传输都是0格式的
          transmitter_vector.push_back(transmitter); // vector不支持引用类型的对象，是深拷贝。暂时先占个位置
          dfs(map, child_x, child_y, transmitter, consumer_vector, transmitter_vector); // 用新的Transmitter作为消息转发
          transmitter_vector[transmitter.id-1] = transmitter; // NOTE: 要放在后面，修改之后重新赋值
        }
      }else{
        // 叶子节点即遇到了Consumer
        Consumer consumer = consumer_vector[node.type_id];
        last.target_vector.push_back({1, consumer.id, consumer.code_format}); // 最后进行消息格式的转换
      }
  }

  // 从Provider开始DFS
  void dfs_tree(Map &map, Provider &provider, vector<Consumer> consumer_vector, vector<Transmitter>& transmitter_vector){
    // 从Provider作为根节点开始深度搜索
    // dfs(map, child_x, child_y, provider, consumer_vector, transmitter_vector);
    // 为了避免在provider处构造Transmitter，在Provider的所有子结点开始搜索
    Node provider_node = map.node_matrix[provider.x][provider.y];
    for(int k=0; k<provider_node.child_xy_list.size(); k++){
      vector<int> child_xy = provider_node.child_xy_list[k];
      int child_x = child_xy[0], child_y = child_xy[1];
      dfs(map, child_x, child_y, provider, consumer_vector, transmitter_vector);
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
// 4
// 2 3 2 0 1 0 1 3 0
// 2 2 2 0 2 0 0 4 0
// 1 2 1 0 3 0
// 1 0 1 1 1 0
// 3 2 1 1 2 0

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
  Map map = Map(G_matrix);                           // 地图map对象
  Provider provider = Provider(I, J);     // 单个Provider对象
  map.node_matrix[I][J].node_type = PROVIDER;
  vector<Consumer> consumer_vector;       // Consumer列表
  vector<Transmitter> transmitter_vector; // Transmitter列表
  for (int i = 0; i < M; i++) {
    Consumer consumer = Consumer(i + 1, C_vector[i][0], C_vector[i][1], C_vector[i][2]);
    consumer_vector.push_back(consumer);
    map.node_matrix[consumer.x][consumer.y].node_type = CONSUMER;
    map.node_matrix[consumer.x][consumer.y].type_id = i;
  }
  Controller controller = Controller(provider, map); // 控制类，作为核心用作计算

  // TODO: 核心部分，路径计算与保存
  // 动态规划计算轨迹
  controller.get_all_trajectory(map, provider, consumer_vector);
  // 轨迹回溯， 轨迹合并，构建树结构
  controller.backward_all_trajectory(map, provider, consumer_vector);
  // 树结构上DFS，并在拐弯处设置Tranmitter，输入输出数据设置，只在Consumer之前的最后一个Tranmitter设置数据转化格式
  controller.dfs_tree(map, provider, consumer_vector, transmitter_vector);

  // cout << "=======================================" << endl;
  // 输出部分
  cout << transmitter_vector.size() << endl;
  // Provider输出
  vector<vector<int>> target_vector = provider.target_vector;
  cout << provider.x << " " << provider.y << " " << target_vector.size();
  for (int j = 0; j < target_vector.size(); j++) {
    cout << " " << target_vector[j][0] << " " << target_vector[j][1] << " "
          << target_vector[j][2];
  }
  cout << endl;
  // Transmitter输出
  for (int i = 0; i < transmitter_vector.size(); i++) {
    Transmitter transmitter = transmitter_vector[i];
    vector<vector<int>> target_vector =
        transmitter.target_vector; // 目标列表，每隔元素为一个三元组
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
