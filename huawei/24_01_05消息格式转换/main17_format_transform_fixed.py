import math
from queue import PriorityQueue

import sys
sys.path += ["../23_12_22路径优化/"]
from main7_map import Provider, Consumer, Node
from main7_map import Transmitter as OldTransmitter


class Transmitter(OldTransmitter):
    def __init__(self, id, x, y):
        super().__init__(id, x, y)
        self.format_dict = {} # 子树下的格式数量字典
        self.need_format = 0

class Map():
    def __init__(self, G_matrix) -> None:
        self.N = len(G_matrix)
        self.node_matrix = []
        for i in range(self.N):
            tmp_v = []
            for j in range(self.N):
                tmp_v.append(Node(i, j, G_matrix[i][j]))
            self.node_matrix.append(tmp_v)
    
    # 这里面的遍历顺序时上右下左，Consumer为中心时没有邻居，应该不能调用这个方法
    def get_neighbor_list(self, x, y):
        # if self.node_matrix[x][y].node_type=="CONSUMER":
        #     # Consumer为中心时没有邻居
        #     return []
        neighbor_list = []
        if x > 0:
            neighbor_list.append([[x-1, y], "DOWN"]) # 当前结点在邻居结点的位置
        if y < self.N-1:
            neighbor_list.append([[x, y+1], "LEFT"])
        if x < self.N-1:
            neighbor_list.append([[x+1, y], "UP"])
        if y > 0:
            neighbor_list.append([[x, y-1], "RIGHT"])
        return neighbor_list
        

class Controller():
    def __init__(self, map, provider, consumer_vector, P) -> None:
        N = map.N
        self.manhattan_scale = int(sum([map.node_matrix[i][j].weight for j in range(N) for i in range(N)])/(N*N)) #曼哈顿距离缩放系数
        self.per_transmitter_cost = 3*P*len(consumer_vector)# 新建一个Transmitter的代价
        print("manhattan_scale", self.manhattan_scale)

    def heuristic(self, x, y, consumer_vector):
        # 到达所有结点最近的曼哈顿距离, 直接曼哈顿距离用到路径权值有点问题，需要手动乘以缩放系数
        manhattan_distance_list = []
        for consumer in consumer_vector:
            manhattan_distance =  abs(x-consumer.x) + abs(y-consumer.y)
            manhattan_distance_list.append(manhattan_distance)
        return min(manhattan_distance_list)*self.manhattan_scale

    # Dijkstra
    def dijkstra_early_stop(self, map, provider, consumer_vector):
        n_customer = len(consumer_vector)
        q = PriorityQueue() # 优先级队列
        q.put((0, (provider.x, provider.y))) # 放入到优先级队列中的元素都是被访问过的，元组第一个数为优先级，第二个数为实际值
        provider_node = map.node_matrix[provider.x][provider.y]
        provider_node.visited = True
        provider_node.distance = 0
        provider_node.best_direction = "UNKOWN"

        while not q.empty():
            current_x, current_y = q.get()[1]
            current_node = map.node_matrix[current_x][current_y]
            if current_node.node_type == "CONSUMER":
                # 邻居及时停止
                if not current_node.visited:
                    n_customer -= 1
                    if n_customer == 0:
                        current_node.visited = True
                        return 
            else:
                # 更新邻居
                for (neighbor_x, neighbor_y), direction in map.get_neighbor_list(current_x, current_y):
                    neighbor_node = map.node_matrix[neighbor_x][neighbor_y]
                    new_cost = current_node.distance + neighbor_node.weight # 以当前结点为中介，该邻居新的距离
                    if (not current_node.node_type == "PROVIDER") and (not (direction == current_node.best_direction)):
                        #  添加转弯代价，从PROVIDER出发肯定不用添加转弯代价
                        new_cost += self.per_transmitter_cost
                    if (neighbor_node.distance==-1) or new_cost < neighbor_node.distance:
                        # 更新从当前结点其到的源点距离
                        priority = new_cost + self.heuristic(neighbor_x, neighbor_y, consumer_vector) # A*改进
                        q.put((priority, (neighbor_x, neighbor_y)))
                        neighbor_node.distance = new_cost
                        neighbor_node.best_direction = direction
            current_node.visited = True 
    
    # 获得所有所有Consumer到Provier的最短路径
    def get_all_trajectory(self, map, provider, consumer_vector):
        print("=======================================")
        self.dijkstra_early_stop(map, provider, consumer_vector)
        for consumer in consumer_vector:
            consumer_node = map.node_matrix[consumer.x][consumer.y]
            print(consumer.id, consumer_node.distance)
            
    # 递归回溯，找父结点构建轨迹
    def backwad_trajectory(self, map, provider, x, y):
        if (x==provider.x and y==provider.y) or (len(map.node_matrix[x][y].child_xy_list)>0):
            return
        else:
            # 构建邻居并继续回溯
            node = map.node_matrix[x][y]
            father_x, father_y = 0, 0
            if node.best_direction == "UNKOWN":
                # 没有路径，直接返回
                return
            elif node.best_direction == "UP":
                father_x = x-1
                father_y = y
            elif node.best_direction == "RIGHT":
                father_x = x
                father_y = y+1
            elif node.best_direction == "DOWN":
                father_x = x+1
                father_y = y
            elif node.best_direction == "LEFT":
                father_x = x
                father_y = y-1
            father_node = map.node_matrix[father_x][father_y]
            self.backwad_trajectory(map, provider, father_node.x, father_node.y)
            father_node.child_xy_list.append([x, y])

    def backward_all_trajectory(self, map, provider, consumer_vector):
        for i in range(len(consumer_vector)):
            consumer = consumer_vector[i]
            self.backwad_trajectory(map, provider, consumer.x, consumer.y)

    # 递归从特定坐标开始DFS
    def dfs(self, map, x, y, last, consumer_vector, transmitter_vector):
        node = map.node_matrix[x][y]
        if len(node.child_xy_list)>=2:
            # 分支点必然拐弯
            transmitter = Transmitter(len(transmitter_vector)+1, node.x, node.y)
            transmitter_vector.append(transmitter)
            node.node_type = "TRANSMITTER"
            node.type_id = transmitter.id
            format_dict = {} # 该分支点上的消息格式字典
            # 遍历所有孩子
            for i in range(len(node.child_xy_list)):
                child_xy = node.child_xy_list[i]
                child_x, child_y = child_xy[0], child_xy[1]
                tmp_format_dict = self.dfs(map, child_x, child_y, transmitter, consumer_vector, transmitter_vector)
                # 合并到现有格式字典上
                for k, v in tmp_format_dict.items():
                    if k in format_dict:
                        format_dict[k] += v
                    else:
                        format_dict[k] = v
            transmitter.format_dict = format_dict
            transmitter.need_format = max(format_dict, key=format_dict.get) # 出现格式次数的最多的消息格式作为该Tranmitter的需求消息格式
            last.target_vector.append([0, transmitter.id, transmitter.need_format]) 
        elif len(node.child_xy_list)==1:
            child_x, child_y = node.child_xy_list[0][0], node.child_xy_list[0][1]
            if (node.best_direction=="UP" and node.x-child_x==-1 and node.y-child_y==0) \
                or (node.best_direction=="RIGHT" and node.x-child_x==0 and node.y-child_y==1)\
                or (node.best_direction=="DOWN" and node.x-child_x==1 and node.y-child_y==0) \
                or (node.best_direction=="LEFT" and node.x-child_x==0 and node.y-child_y==-1):
                # 没有拐弯
                format_dict = self.dfs(map, child_x, child_y, last, consumer_vector, transmitter_vector)
            else:
                # 拐弯了
                transmitter = Transmitter(len(transmitter_vector)+1, node.x, node.y)
                transmitter_vector.append(transmitter)
                node.node_type = "TRANSMITTER"
                node.type_id = transmitter.id
                format_dict = self.dfs(map, child_x, child_y, transmitter, consumer_vector, transmitter_vector)
                transmitter.format_dict = format_dict
                transmitter.need_format = max(format_dict, key=format_dict.get)
                last.target_vector.append([0, transmitter.id, transmitter.need_format]) 
        else:
            # 叶子节点即遇到了Consumer
            consumer = consumer_vector[node.type_id-1]
            format_dict = {consumer.code_format : 1}
            last.target_vector.append([1, consumer.id, consumer.code_format])
        return format_dict

    # 从Provider开始DFS
    def dfs_tree(self, map, provider, consumer_vector, transmitter_vector):
        provider_node = map.node_matrix[provider.x][provider.y]
        for i in range(len(provider_node.child_xy_list)):
            child_xy = provider_node.child_xy_list[i]
            child_x, child_y = child_xy[0], child_xy[1]
            self.dfs(map, child_x, child_y, provider, consumer_vector, transmitter_vector)
        # # provider消息格式都设置为0
        # for i in range(len(provider.target_vector)):
        #     provider.target_vector[i][2] = 0

    # 递归计算time_score
    def calc_tree_time_score(self, map, x, y, transmitter_vector, F_matrix):
        time_score = 0
        node = map.node_matrix[x][y]
        # 对于Tranmitter结点，添加格式转换代价
        if node.node_type == "TRANSMITTER":
            transmitter = transmitter_vector[node.type_id-1]
            # print(f"T{transmitter.id}: {transmitter.need_format}, {transmitter.format_dict}, {transmitter.target_vector}")
            need_format = transmitter.need_format
            tmp_cost = 0
            for target_item in transmitter.target_vector:
                output_format = target_item[2]
                # if not output_format == need_format:
                #     print(need_format, output_format)
                #     tmp_cost += F_matrix[need_format][output_format]
                #     print(need_format, output_format)
                tmp_cost += F_matrix[need_format][output_format] # 相同格式默认cost为0
            print(f"T{transmitter.id}: {transmitter.need_format}; tmp_cost: {tmp_cost}")
            time_score += tmp_cost
        # 路径的代价
        child_xy_list = node.child_xy_list
        for child_x, child_y in child_xy_list:
            time_score += map.node_matrix[child_x][child_y].weight
            time_score += self.calc_tree_time_score(map, child_x, child_y, transmitter_vector, F_matrix)
        return time_score

    # 计算总体代价
    def calc_score(self, map, provider, consumer_vector, transmitter_vector, P, F_matrix):
        N = map.N
        C = len(consumer_vector)
        # 路径代价：递归实现DFS
        time_score = self.calc_tree_time_score(map, provider.x, provider.y, transmitter_vector, F_matrix)/C
        # Tranmitter建造代价：遍历transmitter_vector中各个Transmitter的target_vector数量计算
        cost_score = 0
        for transmitter in transmitter_vector:
            cost_score += (2+len(transmitter.target_vector))*P
        print("time score:", time_score)
        print("cost score:", cost_score)
        score = 2*(10**5)*((1+math.sqrt((time_score+cost_score)/(10*(N**2))))**(-1)) + (10**5)*C
        return score
    

# 原始输入
# 5 3 2 10
# 2 3
# 20 15 10 10 10
# 10 10 10 20 10
# 10 15 10 90 10
# 10 20 10 10 10
# 10 10 10 10 10
# 0 0 0
# 3 1 0
# 1 3 0
# 0 10
# 20 0

# 当前方案输出
# 4
# 2 3 2 0 1 0 1 3 0
# 2 2 2 0 2 0 0 4 0
# 1 2 1 0 3 0
# 1 0 1 1 1 0
# 3 2 1 1 2 0

def main(conf_file=False):
    if conf_file:
        # 从配置文件中读取
        content_list = []
        row = 0
        with open(conf_file, "r") as f:
            content_list = f.readlines()
        N, M, F, P = [int(i) for i in content_list[row].split()]
        row += 1
        G_matrix = [[0 for j in range(N)] for i in range(N)]
        C_vector = [[0 for j in range(3)] for i in range(M)]
        F_matrix = [[0 for j in range(F)] for i in range(F)]
        I, J = [int(i) for i in content_list[row].split()]
        row += 1
        for i in range(N):
            G_matrix[i] = [int(j) for j in content_list[row].split()]
            row += 1
        for i in range(M):
            C_vector[i] = [int(j) for j in content_list[row].split()]
            row += 1
        for i in range(F):
            F_matrix[i] = [int(j) for j in content_list[row].split()]
            row += 1
    else:
        # 从标准输入中读取
        N, M, F, P = [int(i) for i in input().split()]
        G_matrix = [[0 for j in range(N)] for i in range(N)]
        C_vector = [[0 for j in range(3)] for i in range(M)]
        F_matrix = [[0 for j in range(F)] for i in range(F)]
        I, J = [int(i) for i in input().split()]
        for i in range(N):
            G_matrix[i] = [int(j) for j in input().split()]
        for i in range(M):
            C_vector[i] = [int(j) for j in input().split()]
        for i in range(F):
            F_matrix[i] = [int(j) for j in input().split()]

    # 对象构造
    map = Map(G_matrix)
    provider = Provider(I, J)
    map.node_matrix[I][J].node_type = "PROVIDER"
    consumer_vector = []
    transmitter_vector = []
    for i in range(M):
        consumer = Consumer(i+1, C_vector[i][0], C_vector[i][1], C_vector[i][2])
        consumer_vector.append(consumer)
        map.node_matrix[consumer.x][consumer.y].node_type = "CONSUMER"
        map.node_matrix[consumer.x][consumer.y].type_id = i+1
    controller = Controller(map, provider, consumer_vector, P)

    # 核心部分，路径计算与保存
    controller.get_all_trajectory(map, provider, consumer_vector)
    controller.backward_all_trajectory(map, provider, consumer_vector)
    controller.dfs_tree(map, provider, consumer_vector, transmitter_vector)
    
    # 输出部分
    print("=======================================")
    print(len(transmitter_vector))
    target_vector = provider.target_vector
    # Provider输出
    print(provider.x, provider.y, len(target_vector), end=" ")
    for i in range(len(target_vector)):
        print(target_vector[i][0], target_vector[i][1], target_vector[i][2], end=" ")
    print()
    # Transmitter输出
    for i in range(len(transmitter_vector)):
        transmitter = transmitter_vector[i]
        target_vector = transmitter.target_vector
        print(transmitter.x, transmitter.y, len(target_vector), end=" ")
        for j in range(len(target_vector)):
            print(target_vector[j][0], target_vector[j][1], target_vector[j][2], end=" ")
        print()
    
    # 计算得分
    score = controller.calc_score(map, provider, consumer_vector, transmitter_vector, P, F_matrix)
    print("=======================================")
    print(score)
    
    return map, provider, consumer_vector, transmitter_vector


if __name__ == "__main__":
    # 输入部分
    # main()
    # main(conf_file="../23_12_22路径优化/main7_map_input.txt")
    main(conf_file="main16_format_transform.txt")
