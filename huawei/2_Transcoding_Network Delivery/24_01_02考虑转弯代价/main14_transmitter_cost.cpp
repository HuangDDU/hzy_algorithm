#include <iostream>
#include <queue>
#include <vector>
#include <unordered_map>
#include <algorithm>
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
enum direction { UP, RIGHT, DOWN, LEFT, UNKNOWN}; // 动态规划的方向 上， 右， 下， 左， 未知
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
    this->best_direction = UNKNOWN;
    this->distance = -1;
    this->visited = false;
  }
};

// 邻居的坐标和所在位置
class Neighbor{
  public:
    int x;
    int y;
    direction d; // 该邻居在中心结点的位置
    Neighbor(int x, int y, direction d){
      this->x = x;
      this->y = y;
      this->d = d;
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

  // 这里面的遍历顺序时上右下左，Consumer为中心时没有邻居，应该不能调用这个方法
  vector<Neighbor> get_neighbor_vector(int x, int y){
    vector<Neighbor> v;
    if(x>0){
      v.push_back(Neighbor(x-1, y, DOWN));
    }
    if(y<this->N-1){
      v.push_back(Neighbor(x, y+1, LEFT));
    }
    if(x<this->N-1){
      v.push_back(Neighbor(x+1, y, UP));
    }
    if(y>0){
      v.push_back(Neighbor(x, y-1, RIGHT));
    }
    return v;
  }
};


// 核心控制类
class Controller {
public:
  int manhattan_scale; // 曼哈顿缩放系数
  int per_transmitter_cost; // 新建一个Transmitter的代价
  Controller(Provider &provider, Map &node_map, vector<Consumer> consumer_vector, int P) {
    int N = node_map.N;
    int weight_sum = 0;
    for(int i=0; i<N; i++){
      for(int j=0; j<N; j++){
        weight_sum += node_map.node_matrix[i][j].weight;
      }
    }
    this->manhattan_scale = (int) (weight_sum/(N*N));
    this->per_transmitter_cost = 3*P*consumer_vector.size();
  }

  int heuristic(int x,int y, vector<Consumer> consumer_vector){
    // 到达所有结点最近的曼哈顿距离,
    // TODO: 直接曼哈顿距离用到路径权值有点问题，需要手动乘以缩放系数
    vector<int> manhattan_distance_list;
    for(Consumer consumer: consumer_vector){
      int manhattan_distance =  abs(x-consumer.x) + abs(y-consumer.y);
      manhattan_distance_list.push_back(manhattan_distance);
    }
    return *(min_element(manhattan_distance_list.begin(), manhattan_distance_list.end()))*this->manhattan_scale;
  }

  // 小顶堆，优先级队列的元素为坐标，权值为到原点距离
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
  
  // Dijsktra
  void dijkstra_early_stop(Map& node_map, Provider provider, vector<Consumer> consumer_vector){
    int n_consumer = consumer_vector.size();
    priority_queue<Coordinate, vector<Coordinate>, greater<Coordinate>> q;
    q.push(Coordinate(provider.x, provider.y, 0));
    Node& provider_node = node_map.node_matrix[provider.x][provider.y];
    provider_node.visited = true;
    provider_node.distance = 0;
    provider_node.best_direction = UNKNOWN;

    while(!q.empty()){
      Coordinate item = q.top();
      q.pop();
      int current_x=item.x, current_y=item.y;
      Node& current_node = node_map.node_matrix[current_x][current_y];
      if (current_node.node_type == CONSUMER){
        // 邻居及时停止
        if(!current_node.visited){
          n_consumer --;
          if(n_consumer == 0)return;
        }
      }else{
        vector<Neighbor> neighbor_vector = node_map.get_neighbor_vector(current_x, current_y);
        for(Neighbor neighbor: neighbor_vector){
          int neighbor_x=neighbor.x, neighbor_y=neighbor.y;
          Node& neighbor_node = node_map.node_matrix[neighbor_x][neighbor_y];
          int new_cost = current_node.distance + neighbor_node.weight; // 以当前结点为中介，该邻居新的距离
          if(!(neighbor.d == current_node.best_direction)){
            // 添加转弯代价
            new_cost += this->per_transmitter_cost;
          }
          if((neighbor_node.distance==-1)||(new_cost < neighbor_node.distance)){
            int priority = new_cost + this->heuristic(neighbor_x, neighbor_y, consumer_vector); // A*改进
            q.push(Coordinate(neighbor_x, neighbor_y, priority));
            neighbor_node.distance = new_cost;
            neighbor_node.best_direction = neighbor.d;
          }
        }
      }
      current_node.visited = true;
    }
  }

  // 获得所有所有Consumer到Provier的最短路径
  // 预期输出
  // =======================================
  // 1 60
  // 2 40
  // 3 20
  void get_all_trajectory(Map &node_map, Provider &provider, vector<Consumer> consumer_vector) {
    // cout << "=======================================" << endl;
    dijkstra_early_stop(node_map, provider, consumer_vector);
    // for(Consumer consumer: consumer_vector){
    //   Node consumer_node = node_map.node_matrix[consumer.x][consumer.y];
    //   cout << consumer.id << " " << consumer_node.distance << endl;
    // }
  }

  // 递归回溯，找父结点构建轨迹
  void backwad_trajectory(Map &node_map, Provider &provider, int x, int y) {
    if (((x == provider.x) && (y == provider.y)) ||
        (node_map.node_matrix[x][y].child_xy_list.size() > 0)) {
      // 回溯到provider或已经回溯过了，结束停止
      return;
    } else {
      // 构建邻居并继续回溯
      Node &node = node_map.node_matrix[x][y];
      // Node &father_node = node_map.node_matrix[x][y]; // 必须要赋初值先就这样吧
      // if (node.best_direction == UP) {
      //   &father_node = node_map.node_matrix[x - 1][y];
      // } else if (node.best_direction == RIGHT) {
      //   &father_node = node_map.node_matrix[x][y + 1];
      // } else if (node.best_direction == DOWN) {
      //   &father_node = node_map.node_matrix[x + 1][y];
      // } else {
      //   &father_node = node_map.node_matrix[x][y - 1];
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
      Node& father_node = node_map.node_matrix[father_x][father_y];
      backwad_trajectory(node_map, provider, father_node.x, father_node.y);
      father_node.child_xy_list.push_back({x, y});
    }
  }

  // 从所有Consumer作为叶子结点回溯到根节点
  void backward_all_trajectory(Map &node_map, Provider &provider, vector<Consumer> consumer_vector) {
    for (int i = 0; i < consumer_vector.size(); i++) {
      Consumer consumer = consumer_vector[i];
      backwad_trajectory(node_map, provider, consumer.x, consumer.y);
    }
  }

  // 递归从特定坐标开始DFS
  // TODO: DFS中的数据格式可能还是有点问题
  void dfs(Map &node_map, int x, int y, ProviderOrTranmitter &last, vector<Consumer> consumer_vector, vector<Transmitter>& transmitter_vector){
      Node& node = node_map.node_matrix[x][y];
      if(node.child_xy_list.size()>=2){
        // 分支点必然拐弯
        Transmitter transmitter = Transmitter(transmitter_vector.size()+1, node.x, node.y);
        node.node_type = TRANSMITTER;
        node.type_id = transmitter.id;
        last.target_vector.push_back({0, transmitter.id, 0}); // 暂时transmitter之间的传输都是0格式的
        transmitter_vector.push_back(transmitter); // vector不支持引用类型的对象，是深拷贝。暂时先占个位置
        // 遍历所有孩子
        for(int k=0; k<node.child_xy_list.size(); k++){
          vector<int> child_xy = node.child_xy_list[k];
          int child_x = child_xy[0], child_y = child_xy[1];
          dfs(node_map, child_x, child_y, transmitter, consumer_vector, transmitter_vector); // 用新的Transmitter作为消息转发
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
          dfs(node_map, child_x, child_y, last, consumer_vector, transmitter_vector);
        }else{
          // 拐弯了
          Transmitter transmitter = Transmitter(transmitter_vector.size()+1, node.x, node.y);
          node.node_type = TRANSMITTER;
          node.type_id = transmitter.id;
          last.target_vector.push_back({0, transmitter.id, 0}); // 暂时transmitter之间的传输都是0格式的
          transmitter_vector.push_back(transmitter); // vector不支持引用类型的对象，是深拷贝。暂时先占个位置
          dfs(node_map, child_x, child_y, transmitter, consumer_vector, transmitter_vector); // 用新的Transmitter作为消息转发
          transmitter_vector[transmitter.id-1] = transmitter; // NOTE: 要放在后面，修改之后重新赋值
        }
      }else{
        // 叶子节点即遇到了Consumer
        Consumer consumer = consumer_vector[node.type_id-1];
        last.target_vector.push_back({1, consumer.id, consumer.code_format}); // 最后进行消息格式的转换
      }
  }

  // 从Provider开始DFS
  void dfs_tree(Map &node_map, Provider &provider, vector<Consumer> consumer_vector, vector<Transmitter>& transmitter_vector){
    // 从Provider作为根节点开始深度搜索
    // dfs(node_map, child_x, child_y, provider, consumer_vector, transmitter_vector);
    // 为了避免在provider处构造Transmitter，在Provider的所有子结点开始搜索
    Node provider_node = node_map.node_matrix[provider.x][provider.y];
    for(int k=0; k<provider_node.child_xy_list.size(); k++){
      vector<int> child_xy = provider_node.child_xy_list[k];
      int child_x = child_xy[0], child_y = child_xy[1];
      dfs(node_map, child_x, child_y, provider, consumer_vector, transmitter_vector);
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
// 3
// 2 3 1 0 1 0
// 2 2 2 0 2 0 0 3 0
// 0 2 1 1 1 0
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
  Map node_map = Map(G_matrix);                           // 地图map对象
  Provider provider = Provider(I, J);     // 单个Provider对象
  node_map.node_matrix[I][J].node_type = PROVIDER;
  vector<Consumer> consumer_vector;       // Consumer列表
  vector<Transmitter> transmitter_vector; // Transmitter列表
  for (int i = 0; i < M; i++) {
    Consumer consumer = Consumer(i + 1, C_vector[i][0], C_vector[i][1], C_vector[i][2]);
    consumer_vector.push_back(consumer);
    node_map.node_matrix[consumer.x][consumer.y].node_type = CONSUMER;
    node_map.node_matrix[consumer.x][consumer.y].type_id = i+1;
  }
  Controller controller = Controller(provider, node_map, consumer_vector, P); // 控制类，作为核心用作计算

  // TODO: 核心部分，路径计算与保存
  // 动态规划计算轨迹
  controller.get_all_trajectory(node_map, provider, consumer_vector);
  // 轨迹回溯， 轨迹合并，构建树结构
  controller.backward_all_trajectory(node_map, provider, consumer_vector);
  // 树结构上DFS，并在拐弯处设置Tranmitter，输入输出数据设置，只在Consumer之前的最后一个Tranmitter设置数据转化格式
  controller.dfs_tree(node_map, provider, consumer_vector, transmitter_vector);

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
