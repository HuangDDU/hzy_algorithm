#include <iostream>
#include <queue>
#include <vector>
#include <unordered_map>
#include <map>
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
  int need_format;
  unordered_map<int, int> format_dict;
  unordered_map<int, int> local_format_dict;
  Transmitter(int id, int x, int y) {
    this->id = id;
    this->x = x;
    this->y = y;
    this->need_format = 0;
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
  vector<pair<int, direction>> get_neighbor_list(int index){
    int x = index/this->N, y = index%this->N;
    vector<pair<int, direction>> neighbor_list;
    if(x>0){
      neighbor_list.push_back(make_pair((x-1)*this->N+y, DOWN));
    }
    if(y<this->N-1){
      neighbor_list.push_back(make_pair(x*this->N+(y+1), LEFT));
    }
    if(x<this->N-1){
      neighbor_list.push_back(make_pair((x+1)*this->N+y, UP));
    }
    if(y>0){
      neighbor_list.push_back(make_pair(x*this->N+(y-1), RIGHT));
    }
    return neighbor_list;
  }
};


// 核心控制类
class Controller {
public:
  int N;
  Controller(Provider &provider, Map &node_map, vector<Consumer> consumer_vector, int P) {
    this->N = node_map.N;
  }

  // 向源节点回溯
  vector<pair<int, int>> traceback_path(int index, int source, vector<vector<direction>> direction_matrix){
    if(index==source){
      return {};
    }else{
      int x=index/this->N, y=index%this->N;
      int pre_index = 0; // 前驱结点序号
      if(direction_matrix[x][y] == DOWN){
        pre_index = index + this->N;
      }else if(direction_matrix[x][y] == LEFT){
        pre_index = index - 1;
      }else if(direction_matrix[x][y] == UP){
        pre_index = index - this->N;
      }else if(direction_matrix[x][y] == RIGHT){
        pre_index = index + 1;
      }
      vector<pair<int, int>> path = traceback_path(pre_index, source, direction_matrix);
      path.push_back(make_pair(pre_index, index));
      return path;
    }
  }

  // 小顶堆，优先级队列的元素结点序号，权值为结点优先级
  struct NodeIndex{
    int index;
    int weight;

    NodeIndex(int index, int weight) : index(index), weight(weight) {}

    // 定义比较函数
    bool operator>(const NodeIndex& other) const {
        return weight > other.weight;
    } 
  };
  
  pair<map<pair<int, int>, vector<pair<int, int>>>, map<pair<int, int>, int>> dijkstra(Map &node_map, int source, vector<int> consumer_index_list){
    vector<vector<int>> distance_matrix(this->N, vector<int>(this->N, -1)); // 距离矩阵
    vector<vector<direction>> direction_matrix(this->N, vector<direction>(this->N, UNKNOWN)); // 方向矩阵用于回溯
    vector<vector<bool>> visited_matrix(this->N, vector<bool>(this->N, false)); // 访问矩阵
    int n_consumer = consumer_index_list.size(); // 用作记录及时停止

    priority_queue<NodeIndex, vector<NodeIndex>, greater<NodeIndex>> q;
    q.push(NodeIndex(source, 0));
    int source_x = source/this->N, source_y = source%this->N;
    distance_matrix[source_x][source_y] = 0; // 起点设置为0
    direction_matrix[source_x][source_y] = UNKNOWN;
    visited_matrix[source_x][source_y] = true;

    while(!q.empty()){
      int current_index = q.top().index;
      q.pop();
      int current_x = current_index/this->N, current_y = current_index%this->N;
      Node current_node = node_map.node_matrix[current_x][current_y];
      auto it = find(consumer_index_list.begin(), consumer_index_list.end(), current_index);
      if(!(it==consumer_index_list.end())){
        // 邻居及时停止
        if(!(visited_matrix[current_x][current_y])){
          n_consumer--;
          if(n_consumer==0){
            visited_matrix[current_x][current_y] = true;
            break;
          }
        }
      }else{
        // 更新邻居
        for(auto p: node_map.get_neighbor_list(current_index)){
          int neighbor_index = p.first;
          direction d = p.second;
          int neighbor_x = neighbor_index/this->N, neighbor_y = neighbor_index%this->N;
          Node neighbor_node = node_map.node_matrix[neighbor_x][neighbor_y];
          int new_cost = distance_matrix[current_node.x][current_node.y] + neighbor_node.weight;
          // 添加转弯代价
          if ((distance_matrix[neighbor_x][neighbor_y] == -1) or (new_cost < distance_matrix[neighbor_x][neighbor_y])){
            // 更新从当前结点其到的源点距离
            int priority = new_cost; // TODO:A*改进
            q.push(NodeIndex(neighbor_index, priority));
            distance_matrix[neighbor_x][neighbor_y] = new_cost;
            direction_matrix[neighbor_x][neighbor_y]= d;
          }
        }
      }
       visited_matrix[current_x][current_y] = true;
    }

    // 回溯找路径
    map<pair<int, int>, vector<pair<int, int>>> path_dict;
    map<pair<int, int>, int> cost_dict;
    for(int consumer_index : consumer_index_list){
      vector<pair<int, int>> path = traceback_path(consumer_index, source, direction_matrix);
      path_dict[make_pair(source, consumer_index)] = path;
      cost_dict[make_pair(source, consumer_index)] = distance_matrix[consumer_index/this->N][consumer_index%this->N];
    }
    return make_pair(path_dict, cost_dict);
  }

  // 小顶堆，优先级队列的元素为边的源、目，权值为边权
  struct Edge {
    pair<int, int> edge;
    int weight;

    Edge(pair<int, int> p, int weight) : edge(p), weight(weight) {}

    // 定义比较函数
    bool operator>(const Edge& other) const {
        return weight > other.weight;
    }
  };

  void mph(Map &node_map, Provider &provider, vector<Consumer> consumer_vector){
    // 坐标转化为序号
    vector<int> consumer_index_list;
    for(Consumer consumer: consumer_vector){
      consumer_index_list.push_back(consumer.x*this->N+consumer.y);
    }
    int provider_index = provider.x*this->N+provider.y;
    
    // 组播树
    vector<int> V_list = {provider_index};
    vector<pair<int, int>> G_list = {};
    vector<int> new_V_list = {provider_index};
    map<pair<int, int>, vector<pair<int, int>>> path_dict; // 这里只能用order_map
    priority_queue<Edge, vector<Edge>, greater<Edge>> path_cost_q;

    int n = consumer_index_list.size();
    for(int i=0; i<n; i++){
      // 上一次新加入的中间结点计算到组播树外Consumer的最短路径
      for(int new_V :new_V_list){
        auto p = dijkstra(node_map, new_V, consumer_index_list);
        map<pair<int, int>, vector<pair<int, int>>> new_path_dict = p.first;
        map<pair<int, int>, int> new_path_cost_dict = p.second;
        for(auto p2:new_path_dict){
          path_dict[p2.first] = p2.second;
        }
        for(auto p2:new_path_cost_dict){
          path_cost_q.push(Edge(p2.first, p2.second));
        }

      }

      // 获取距离当前组播树最近的Consumer的path
      vector<pair<int, int>> path;
      while((!(path_cost_q.empty()))&&(path.size()==0)){
        // 提取到一条有效路径停止
        path = path_dict[path_cost_q.top().edge];
        path_cost_q.pop();

      }
      if(path.size()==0){
        // 无法往组播树里添加Consumer了, 现有Consumer无法与已有组播树相连，则及时停止
        break;
      }

      // 更新组播树
      vector<int> new_V_list;
      for(int j=0; j<path.size(); j++){
        G_list.push_back(path[j]);
        int target = path[j].second;
        V_list.push_back(target);
        if (j < path.size()-1){
          new_V_list.push_back(target);
        }
      }

      // 清除组播树中每个中间结点到新Consumer的最短路径
      // int new_consumer_index = path[path.size()-1].second;
      int new_consumer_index = (path.end()-1)->second;
      for(auto p: path_dict){
        if(p.first.second == new_consumer_index){
          // p.second = {};
          path_dict[p.first] = {};
        }
      }
      auto new_end = remove(consumer_index_list.begin(), consumer_index_list.end(), new_consumer_index);
      consumer_index_list.erase(new_end, consumer_index_list.end());
    }

    // 树结构保存Map对象中
    for(int i=0; i<G_list.size(); i++){
      pair<int, int> p= G_list[i];
      int u = p.first, v = p.second;
      int u_x = u/this->N, u_y = u%this->N;
      int v_x = v/this->N, v_y = v%this->N;
      node_map.node_matrix[u_x][u_y].child_xy_list.push_back({v_x, v_y});
      direction best_direction = UNKNOWN;
      if ((u_x-v_x==1)&&(u_y-v_y==0)){
        best_direction = DOWN;
      }else if((u_x-v_x==0)&&(u_y-v_y==-1)){
        best_direction = LEFT;
      }else if((u_x-v_x==-1)&&(u_y-v_y==0)){
        best_direction = UP;
      }else if((u_x-v_x==0)&&(u_y-v_y==1)){
        best_direction = RIGHT;
      }
      node_map.node_matrix[v_x][v_y].best_direction = best_direction;
    }
  }
  

  // 递归从特定坐标开始DFS
  pair<int, unordered_map<int, int>> dfs(Map &node_map, int x, int y, ProviderOrTranmitter &last, vector<Consumer> consumer_vector, vector<Transmitter>& transmitter_vector, vector<vector<int>> F_matrix){
      Node& node = node_map.node_matrix[x][y];
      unordered_map<int, int> format_dict;
      int need_format = 0;
      if(node.child_xy_list.size()>=2){
        // 分支点必然拐弯
        Transmitter transmitter = Transmitter(transmitter_vector.size()+1, node.x, node.y);
        transmitter_vector.push_back(transmitter); // vector不支持引用类型的对象，是深拷贝。暂时先占个位置
        node.node_type = TRANSMITTER;
        node.type_id = transmitter.id;
        unordered_map<int, int> local_format_dict; //该分支点局部的消息格式字典
        // 遍历所有孩子
        for(int k=0; k<node.child_xy_list.size(); k++){
          vector<int> child_xy = node.child_xy_list[k];
          int child_x = child_xy[0], child_y = child_xy[1];
          int need_format;
          unordered_map<int, int> tmp_format_dict;
          pair<int, unordered_map<int, int>> result_pair = dfs(node_map, child_x, child_y, transmitter, consumer_vector, transmitter_vector, F_matrix); // 用新的Transmitter作为消息转发
          need_format = result_pair.first;
          tmp_format_dict = result_pair.second;
          local_format_dict[need_format] += 1;
          for(auto& pair :tmp_format_dict){
            format_dict[pair.first] += pair.second;
          }
        }
        transmitter.format_dict = format_dict;
        transmitter.local_format_dict = local_format_dict;
        // TODO: 替换掉完全由数量决定的输入格式的逻辑
        // auto max_element = format_dict.begin();
        // for (auto it = format_dict.begin(); it != format_dict.end(); ++it) {
        //     if (it->second > max_element->second) {
        //         max_element = it;
        //     }
        // }
        // transmitter.need_format = max_element->first;
        // TODO: 寻找代价最小的输入格式
        vector<int> format_list;
        for(auto& p: local_format_dict){
          format_list.push_back(p.first);
        }
        int best_need_format =  format_list[0];
        int best_need_format_cost = -1;
        for(int tmp_need_format: format_list){
          int tmp_need_format_cost = 0;
          for(auto& p: local_format_dict){
            int k=p.first, v=p.second;
            tmp_need_format_cost += F_matrix[tmp_need_format][k]*v;
          }
          if ((best_need_format_cost==-1)||((!(best_need_format_cost == -1))&&(tmp_need_format_cost < best_need_format_cost))){
            best_need_format = tmp_need_format;
            best_need_format_cost = tmp_need_format_cost;
          }
        }
        need_format = best_need_format;
        transmitter.need_format = need_format;
        transmitter_vector[transmitter.id-1] = transmitter;
        last.target_vector.push_back({0, transmitter.id, need_format});
      }else if(node.child_xy_list.size()==1){
        int child_x = node.child_xy_list[0][0], child_y = node.child_xy_list[0][1];
        if(
          ((node.best_direction==UP)&&(node.x-child_x==-1)&&(node.y-child_y==0))||
          ((node.best_direction==RIGHT)&&(node.x-child_x==0)&&(node.y-child_y==1))||
          ((node.best_direction==DOWN)&&(node.x-child_x==1)&&(node.y-child_y==0))||
          ((node.best_direction==LEFT)&&(node.x-child_x==0)&&(node.y-child_y==-1))
        ){
          // 没有拐弯
          pair<int, unordered_map<int, int>> result_pair = dfs(node_map, child_x, child_y, last, consumer_vector, transmitter_vector, F_matrix);
          need_format = result_pair.first;
          format_dict = result_pair.second;
        }else{
          // 拐弯了
          Transmitter transmitter = Transmitter(transmitter_vector.size()+1, node.x, node.y);
          node.node_type = TRANSMITTER;
          node.type_id = transmitter.id;
          transmitter_vector.push_back(transmitter); // vector不支持引用类型的对象，是深拷贝。暂时先占个位置
          pair<int, unordered_map<int, int>> result_pair = dfs(node_map, child_x, child_y, transmitter, consumer_vector, transmitter_vector, F_matrix); // 用新的Transmitter作为消息转发
          need_format = result_pair.first;
          format_dict = result_pair.second;
          transmitter.format_dict = format_dict;
          transmitter.need_format = need_format;
          transmitter.local_format_dict[need_format] = 1;
          transmitter_vector[transmitter.id-1] = transmitter;
          last.target_vector.push_back({0, transmitter.id, need_format});
        }
      }else{
        // 叶子节点即遇到了Consumer
        Consumer consumer = consumer_vector[node.type_id-1];
        format_dict[consumer.code_format] = 1; 
        need_format = consumer.code_format;
        last.target_vector.push_back({1, consumer.id, consumer.code_format}); // 最后进行消息格式的转换
      }
      return make_pair(need_format, format_dict);
  }

  // 从Provider开始DFS
  void dfs_tree(Map &node_map, Provider &provider, vector<Consumer> consumer_vector, vector<Transmitter>& transmitter_vector, vector<vector<int>> F_matrix){
    // 从Provider作为根节点开始深度搜索
    // dfs(node_map, child_x, child_y, provider, consumer_vector, transmitter_vector);
    // 为了避免在provider处构造Transmitter，在Provider的所有子结点开始搜索
    Node provider_node = node_map.node_matrix[provider.x][provider.y];
    for(int k=0; k<provider_node.child_xy_list.size(); k++){
      vector<int> child_xy = provider_node.child_xy_list[k];
      int child_x = child_xy[0], child_y = child_xy[1];
      dfs(node_map, child_x, child_y, provider, consumer_vector, transmitter_vector, F_matrix);
    }
    // // provider消息格式都设置为0
    // for(int k=0; k<provider.target_vector.size(); k++){
    //   provider.target_vector[k][2] = 0;
    // }
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
  controller.mph(node_map, provider, consumer_vector);
  // 树结构上DFS，并在拐弯处设置Tranmitter，输入输出数据设置，只在Consumer之前的最后一个Tranmitter设置数据转化格式
  controller.dfs_tree(node_map, provider, consumer_vector, transmitter_vector, F_matrix);

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
