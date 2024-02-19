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
        self.local_format_dict = {} # 当前Tranmitter输出的格式数量字典
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
    
    # 获得邻居列表
    def get_neighbor_list(self, index):
        x, y = int(index/self.N), index%self.N
        neighbor_list = []
        if x > 0:
            neighbor_list.append([(x-1)*self.N+y, "DOWN"]) # 当前结点在邻居结点的位置
        if y < self.N-1:
            neighbor_list.append([x*self.N+(y+1), "LEFT"])
        if x < self.N-1:
            neighbor_list.append([(x+1)*self.N+y, "UP"])
        if y > 0:
            neighbor_list.append([x*self.N+(y-1), "RIGHT"])
        return neighbor_list
        

class Controller():
    def __init__(self, map, provider, consumer_vector, P) -> None:
        self.N = map.N
        N = self.N
        self.manhattan_scale = int(sum([map.node_matrix[i][j].weight for j in range(N) for i in range(N)])/(N*N)) #曼哈顿距离缩放系数
        self.per_transmitter_cost = 3*P*len(consumer_vector)# 新建一个Transmitter的代价
        print("manhattan_scale", self.manhattan_scale)

    def heuristic(self, x, y, consumer_index_list):
        # 到达所有结点最近的曼哈顿距离, 直接曼哈顿距离用到路径权值有点问题，需要手动乘以缩放系数
        manhattan_distance_list = []
        for consumer_index in consumer_index_list:
            consumer_x, consumer_y = int(consumer_index/self.N), consumer_index%self.N
            manhattan_distance =  abs(x-consumer_x) + abs(y-consumer_y)
            manhattan_distance_list.append(manhattan_distance)
        return min(manhattan_distance_list)*self.manhattan_scale
    
    # 输出矩阵
    def show_map(self, N, distance_matrix, direction_matrix):
        for i in range(N):
            for j in range(N):
                print(distance_matrix[i][j], end=" ")
            print()

        print("===============")
        for i in range(N):
            for j in range(N):
                direction = direction_matrix[i][j]
                if direction == "UP":
                    print("↑", end=" ")
                elif direction == "RIGHT":
                    print("→", end=" ")            
                elif direction == "DOWN":
                    print("↓", end=" ")            
                elif direction == "LEFT":
                    print("←", end=" ")
                else:
                    print("U", end=" ")
            print()
        print("==================================")

    # 向源节点回溯
    def traceback_path(self, index, source, direction_matrix):
        if index == source:
            return []
        else:
            x, y = int(index/self.N), index%self.N
            pre_index = 0 # 前驱结点序号
            if direction_matrix[x][y] == "DOWN":
                pre_index = index + self.N
            elif direction_matrix[x][y] == "LEFT":
                pre_index = index -1
            elif direction_matrix[x][y] == "UP":
                pre_index = index - self.N
            elif direction_matrix[x][y] == "RIGHT":
                pre_index = index + 1
            pre_path = self.traceback_path(pre_index, source, direction_matrix)
            path = pre_path + [(pre_index, index)]
            return path

    # 从source执行dijkstra，返回到consumer的路径
    def dijkstra(self, map, source, consumer_index_list, all_consumer_index_list):
        # 初始化
        distance_matrix = [[-1 for i in range(self.N)] for j in range(self.N)] # 距离矩阵
        direction_matrix = [["" for i in range(self.N)] for j in range(self.N)] # 方向矩阵用于回溯
        direction_list_matrix = [[[] for i in range(self.N)] for j in range(self.N)] # 可能会有多个同等代价的候选前驱，后续需要选择代价最低的
        visited_matrix = [[False for i in range(self.N)] for j in range(self.N)] # 访问矩阵
        n_consumer = len(consumer_index_list) # 用作记录及时停止

        q = PriorityQueue()
        q.put((0, source))
        source_x, source_y = int(source/self.N), source%self.N
        distance_matrix[source_x][source_y] = 0 # 起点设置为0
        direction_matrix[source_x][source_y] = "UNKOWN"
        visited_matrix[source_x][source_y] = True

        while not q.empty():
            current_index = q.get()[1]
            current_x, current_y = int(current_index/self.N), current_index%self.N # 转化为坐标方便查找
            current_node = map.node_matrix[current_x][current_y]
            if current_index in all_consumer_index_list:
                if not current_index in consumer_index_list:
                    # 该位置为已经找到路径Consumer，直接跳过
                    continue
                else:
                    # 邻居及时停止
                    if not visited_matrix[current_x][current_y]:
                        n_consumer -= 1
                        if n_consumer == 0:
                            visited_matrix[current_x][current_y] = True
                            break
            else:
                # 更新邻居
                for neighbor_index, direction in map.get_neighbor_list(current_index):
                    neighbor_x, neighbor_y = int(neighbor_index/self.N), neighbor_index%self.N
                    neighbor_node = map.node_matrix[neighbor_x][neighbor_y]
                    new_cost = distance_matrix[current_node.x][current_node.y] + neighbor_node.weight
                    # 添加转弯代价
                    # if (not current_index == source) and (not (direction == direction_matrix[current_x][current_y])):
                    if (not current_index == source) :
                        #  添加转弯代价，从起点出发肯定不用添加转弯代价
                        trun = True
                        for d in direction_list_matrix[current_x][current_y]:
                            if direction == d:
                                # 有一个候选方向没有转弯，最好的路径
                                trun = False
                                direction_matrix[current_x][current_y] = d
                                break
                        if trun:
                            new_cost += self.per_transmitter_cost
                    if (distance_matrix[neighbor_x][neighbor_y] == -1) or (new_cost <= distance_matrix[neighbor_x][neighbor_y]):
                        # 这里同等权重也要更新
                        # 更新从当前结点其到的源点距离
                        priority = new_cost + self.heuristic(neighbor_x, neighbor_y, consumer_index_list) # A*改进
                        q.put((priority, neighbor_index))
                        distance_matrix[neighbor_x][neighbor_y] = new_cost
                        direction_matrix[neighbor_x][neighbor_y] = direction
                        direction_list_matrix[neighbor_x][neighbor_y].append(direction) # 添加候选一个方向
            visited_matrix[current_x][current_y] = True
        # print(source, ":", int(source/self.N), source%self.N)
        # self.show_map(self.N, distance_matrix, direction_matrix)
    
        # 回溯找路径
        path_dict = {}
        cost_dict = {}
        for consumer_index in consumer_index_list:
            cost = distance_matrix[int(consumer_index/self.N)][consumer_index%self.N]
            if not cost == -1:
                # 有些结点不可达则跳过
                path = self.traceback_path(consumer_index, source, direction_matrix)
                path_dict[(source, consumer_index)] = path
                cost_dict[(source, consumer_index)] = cost
        return path_dict, cost_dict


    def mph(self, map, provider, consumer_vector):
        # 坐标转化为序号
        consumer_index_list = [consumer.x*self.N+consumer.y for consumer in consumer_vector] # 未找到路径的consumer列表
        all_consumer_index_list = consumer_index_list.copy() # 所有consumer列表
        provider_index = provider.x*self.N+provider.y 
        
        # 组播树
        V_list = [provider_index]
        G_list  = []
        new_V_list =  [provider_index] # 待计算最短距离的
        path_dict = {} # 组播树中每个中间结点到组播树外Consumer的最短路径
        path_cost_q =  PriorityQueue() # 组播树中每个中间结点到Consumer的成本优先级队列（不能只保存最短的距离）
        
        for i in range(len(consumer_index_list)):
            # 选择距离组播树最近Consumer，加入组播树，直到所有的结点完成
            # 上一次新加入的中间结点计算到组播树外Consumer的最短路径
            for new_V in new_V_list:
                new_path_dict, new_path_cost_dict = self.dijkstra(map, new_V, consumer_index_list, all_consumer_index_list)
                for k, v in new_path_dict.items():
                    path_dict[k] = v
                for k, v in new_path_cost_dict.items():
                    path_cost_q.put((v, k))

            # 获取距离当前组播树最近的Consumer的path
            path = []
            while (not path_cost_q.empty()) and (len(path)==0):
                # 提取到一条有效路径停止
                k = path_cost_q.get()[1]
                path = path_dict[k]
            if len(path) == 0:
                # 无法往组播树里添加Consumer了, 现有Consumer无法与已有组播树相连，则及时停止
                break
            
            # 更新组播树
            new_V_list = []
            for j in range(len(path)):
                G_list.append(path[j])
                target = path[j][1]
                V_list.append(target)
                if j < len(path)-1:
                    # 路径中的结点可以用来扩展组播树
                    new_V_list.append(target)
            
            # 清除组播树中每个中间结点到新Consumer的最短路径
            new_consumer_index = path[-1][1]
            for k in path_dict:
                if new_consumer_index in k:
                    path_dict[k] = []
            consumer_index_list.remove(new_consumer_index)
            
        # print(V_list)
        # print(G_list)

        # 树结构保存map对象中
        for u,v in G_list:
            u_x, u_y = int(u/self.N), u%self.N
            v_x, v_y = int(v/self.N), v%self.N
            map.node_matrix[u_x][u_y].child_xy_list.append((v_x, v_y))
            best_direction = "UNKOWN"
            if (u_x-v_x==1) and (u_y-v_y==0):
                best_direction = "DOWN"
            elif (u_x-v_x==0) and (u_y-v_y==-1):
                best_direction = "LEFT"
            elif (u_x-v_x==-1) and (u_y-v_y==0):
                best_direction = "UP"
            elif (u_x-v_x==0) and (u_y-v_y==1):
                best_direction = "RIGHT"
            map.node_matrix[v_x][v_y].best_direction = best_direction

    # 递归从特定坐标开始DFS
    def dfs(self, map, x, y, last, consumer_vector, transmitter_vector, F_matrix):
        node = map.node_matrix[x][y]
        format_dict = {} # 该分支点子树的消息格式字典
        need_format = 0
        if len(node.child_xy_list)>=2:
            # 分支点必然拐弯
            transmitter = Transmitter(len(transmitter_vector)+1, node.x, node.y)
            transmitter_vector.append(transmitter)
            node.node_type = "TRANSMITTER"
            node.type_id = transmitter.id
            local_format_dict = {} # 该分支点局部的消息格式字典
            # 遍历所有孩子
            for i in range(len(node.child_xy_list)):
                child_xy = node.child_xy_list[i]
                child_x, child_y = child_xy[0], child_xy[1]
                need_format, tmp_format_dict = self.dfs(map, child_x, child_y, transmitter, consumer_vector, transmitter_vector, F_matrix)
                if need_format in local_format_dict:
                    local_format_dict[need_format] += 1
                else:
                    local_format_dict[need_format] = 1
                # 合并到现有格式字典上
                for k, v in tmp_format_dict.items():
                    if k in format_dict:
                        format_dict[k] += v
                    else:
                        format_dict[k] = v
            transmitter.format_dict = format_dict
            transmitter.local_format_dict = local_format_dict
            # TODO: 替换掉完全由数量决定的输入格式的逻辑
            # transmitter.need_format = max(format_dict, key=format_dict.get) # 出现格式次数的最多的消息格式作为该Tranmitter的需求消息格式
            # TODO: 寻找代价最小的输入格式
            format_list = list(local_format_dict.keys())
            best_need_format =  format_list[0]
            best_need_format_cost = -1
            for tmp_need_format  in format_list:
                # 尝试各种输入格式
                tmp_need_format_cost = 0
                for k, v in local_format_dict.items():
                    tmp_need_format_cost+= F_matrix[tmp_need_format][k]*v
                if (best_need_format_cost == -1) or ((not (best_need_format_cost == -1)) and (tmp_need_format_cost < best_need_format_cost)):
                    # 找到了代价更小的输入格式
                    best_need_format = tmp_need_format
                    best_need_format_cost = tmp_need_format_cost
            need_format = best_need_format # 用作后续递归返回结果
            transmitter.need_format = need_format
            last.target_vector.append([0, transmitter.id, need_format])
        elif len(node.child_xy_list)==1:
            child_x, child_y = node.child_xy_list[0][0], node.child_xy_list[0][1]
            if (node.best_direction=="UP" and node.x-child_x==-1 and node.y-child_y==0) \
                or (node.best_direction=="RIGHT" and node.x-child_x==0 and node.y-child_y==1)\
                or (node.best_direction=="DOWN" and node.x-child_x==1 and node.y-child_y==0) \
                or (node.best_direction=="LEFT" and node.x-child_x==0 and node.y-child_y==-1):
                # 没有拐弯
                need_format, format_dict = self.dfs(map, child_x, child_y, last, consumer_vector, transmitter_vector, F_matrix)
            else:
                # 拐弯了
                transmitter = Transmitter(len(transmitter_vector)+1, node.x, node.y)
                transmitter_vector.append(transmitter)
                node.node_type = "TRANSMITTER"
                node.type_id = transmitter.id
                need_format, format_dict = self.dfs(map, child_x, child_y, transmitter, consumer_vector, transmitter_vector, F_matrix)
                transmitter.format_dict = format_dict
                transmitter.need_format = need_format
                transmitter.local_format_dict[need_format] = 1
                last.target_vector.append([0, transmitter.id, need_format])
        else:
            # 叶子节点即遇到了Consumer
            consumer = consumer_vector[node.type_id-1]
            format_dict = {consumer.code_format : 1}
            need_format = consumer.code_format
            last.target_vector.append([1, consumer.id, consumer.code_format])
        return need_format, format_dict

    # 从Provider开始DFS
    def dfs_tree(self, map, provider, consumer_vector, transmitter_vector, F_matrix):
        provider_node = map.node_matrix[provider.x][provider.y]
        for i in range(len(provider_node.child_xy_list)):
            child_xy = provider_node.child_xy_list[i]
            child_x, child_y = child_xy[0], child_xy[1]
            self.dfs(map, child_x, child_y, provider, consumer_vector, transmitter_vector, F_matrix)
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
    controller.mph(map, provider, consumer_vector)
    controller.dfs_tree(map, provider, consumer_vector, transmitter_vector, F_matrix)
    
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
    # main(conf_file="main19_mph_map.txt")
    # main(conf_file="../24_01_05消息格式转换/main16_format_transform3.txt")
    # main(conf_file="../24_01_05消息格式转换/main18_format_transform_weight.txt")
    # main(conf_file="main19_mph_map_new.txt")
    main(conf_file="../24_01_02考虑转弯代价/main15_other_map1.txt")