from enum import Enum


class ProviderOrTransmitter:
    def __init__(self) -> None:
        self.x = 0
        self.y = 0
        self.target_vector = []
    

class Provider(ProviderOrTransmitter):
    def __init__(self, x, y) -> None:
        super().__init__()
        self.x = x
        self.y = y


class Transmitter(ProviderOrTransmitter):
    def __init__(self, id, x, y) -> None:
        super().__init__()
        self.id = id
        self.x = x
        self.y = y


class Consumer():
    def __init__(self, id, x, y, code_format) -> None:
        self.id = id
        self.x = x
        self.y = y
        self.code_format = code_format


class Node():
    def __init__(self, x, y, weight) -> None:
        self.x = x
        self.y = y
        self.weight = weight
        self.node_type = "EMPTY" # 枚举类型直接用字符串了。PROVIDER, CONSUMER, TRANSMITTER, EMPTY
        self.type_id = -1
        self.best_direction = "UNKOWN" # UP, RIGHT, DOWN, LEFT, UNKOWN
        self.distance = -1
        self.visited = False
        self.child_xy_list = []


class Map():
    def __init__(self, G_matrix) -> None:
        self.N = len(G_matrix)
        self.node_matrix = []
        for i in range(N):
            tmp_v = []
            for j in range(N):
                tmp_v.append(Node(i, j, G_matrix[i][j]))
            self.node_matrix.append(tmp_v)
                
        

class Controller():
    def __init__(self, provider, map) -> None:
        provider_node = map.node_matrix[provider.x][provider.y]
        provider_node.visited = True
        provider_node.distance = 0
        # 十字区域的distance设置
        for i in range(provider.x-1, -1, -1):
            node = map.node_matrix[i][provider.y]
            pre_node = map.node_matrix[i+1][provider.y]
            node.visited = True
            node.best_direction = "DOWN"
            node.distance = node.weight + pre_node.distance
        for i in range(provider.x+1, map.N):
            node = map.node_matrix[i][provider.y]
            pre_node = map.node_matrix[i-1][provider.y]
            node.visited = True
            node.best_direction = "UP"
            node.distance = node.weight + pre_node.distance
        for i in range(provider.y-1, -1, -1):
            node = map.node_matrix[provider.x][i]
            pre_node = map.node_matrix[provider.x][i+1]
            node.visited = True
            node.best_direction = "RIGHT"
            node.distance = node.weight + pre_node.distance
        for i in range(provider.y+1, map.N):
            node = map.node_matrix[provider.x][i]
            pre_node = map.node_matrix[provider.x][i-1]
            node.visited = True
            node.best_direction = "LEFT"
            node.distance = node.weight + pre_node.distance
        
    #  递归计算距离
    # TODO: 路径上可能有些结点是Consumer，不能穿透
    def get_distance(self, map, provider, x, y):
        node = map.node_matrix[x][y]
        if node.visited:
            # 直接访问
            return node.distance
        else:
            # 根据坐标确定动态规划的方向
            if (x < provider.x) and (y < provider.y):
                # 当前结点在provider的左上角
                down_distance = self.get_distance(map, provider, x+1, y)
                right_distance = self.get_distance(map, provider, x, y + 1)
                if down_distance <= right_distance:
                    node.best_direction = "DOWN"
                    node.distance = down_distance + node.weight
                else:
                    node.best_direction = "RIGHT"
                    node.distance = right_distance + node.weight
            elif (x > provider.x) and (y < provider.y):
                # 当前结点在provider的左下角
                up_distance = self.get_distance(map, provider, x - 1, y)
                right_distance = self.get_distance(map, provider, x, y + 1)
                if up_distance <= right_distance:
                    node.best_direction = "UP"
                    node.distance = up_distance + node.weight
                else:
                    node.best_direction = "RIGHT"
                    node.distance = right_distance + node.weight
            elif (x < provider.x) and (y > provider.y):
                # 当前结点在provider的右上角
                down_distance = self.get_distance(map, provider, x + 1, y)
                left_distance = self.get_distance(map, provider, x, y - 1)
                if down_distance <= left_distance:
                    node.best_direction = "DOWN"
                    node.distance = down_distance + node.weight
                else:
                    node.best_direction = "LEFT"
                    node.distance = left_distance + node.weight
            else:
                # 当前结点在provider的右下角
                up_distance = self.get_distance(map, provider, x - 1, y)
                left_distance = self.get_distance(map, provider, x, y - 1)
                if up_distance <= left_distance:
                    node.best_direction = "UP"
                    node.distance = up_distance + node.weight
                else:
                    node.best_direction = "LEFT"
                    node.distance = left_distance + node.weight
            node.visited = True
        return node.distance

    # 获得所有所有Consumer到Provier的最短路径
    # 预期输出
    # =======================================
    # 1 60
    # 2 40
    # 3 20
    def get_all_trajectory(self, map, provider, consumer_vector):
        print("=======================================")
        for i in range(len(consumer_vector)):
            consumer = consumer_vector[i]
            distance = self.get_distance(map, provider, consumer.x, consumer.y)
            print(consumer.id, distance)

    # 递归回溯，找父结点构建轨迹
    def backwad_trajectory(self, map, provider, x, y):
        if (x==provider.x and y==provider.y) or (len(map.node_matrix[x][y].child_xy_list)>0):
            return
        else:
            # 构建邻居并继续回溯
            node = map.node_matrix[x][y]
            father_x, father_y = 0, 0
            if node.best_direction == "UP":
                father_x = x-1
                father_y = y
            elif node.best_direction == "RIGHT":
                father_x = x
                father_y = y+1
            elif node.best_direction == "DOWN":
                father_x = x+1
                father_y = y
            else:
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
            last.target_vector.append([0, transmitter.id, 0])
            transmitter_vector.append(transmitter)
            # 遍历所有孩子
            for i in range(len(node.child_xy_list)):
                child_xy = node.child_xy_list[i]
                child_x, child_y = child_xy[0], child_xy[1]
                self.dfs(map, child_x, child_y, transmitter, consumer_vector, transmitter_vector)
        elif len(node.child_xy_list)==1:
            child_x, child_y = node.child_xy_list[0][0], node.child_xy_list[0][1]
            if (node.best_direction=="UP" and node.x-child_x==-1 and node.y-child_y==0) \
                or (node.best_direction=="RIGHT" and node.x-child_x==0 and node.y-child_y==1)\
                or (node.best_direction=="DOWN" and node.x-child_x==1 and node.y-child_y==0) \
                or (node.best_direction=="LEFT" and node.x-child_x==0 and node.y-child_y==-1):
                # 没有拐弯
                self.dfs(map, child_x, child_y, last, consumer_vector, transmitter_vector)
            else:
                # 拐弯了
                transmitter = Transmitter(len(transmitter_vector)+1, node.x, node.y)
                last.target_vector.append([0, transmitter.id, 0])
                transmitter_vector.append(transmitter)
                self.dfs(map, child_x, child_y, transmitter, consumer_vector, transmitter_vector)
        else:
            # 叶子节点即遇到了Consumer
            consumer = consumer_vector[node.type_id]
            last.target_vector.append([1, consumer.id, consumer.code_format])


    # 从Provider开始DFS
    def dfs_tree(self, map, provider, consumer_vector, transmitter_vector):
        provider_node = map.node_matrix[provider.x][provider.y]
        for i in range(len(provider_node.child_xy_list)):
            child_xy = provider_node.child_xy_list[i]
            child_x, child_y = child_xy[0], child_xy[1]
            self.dfs(map, child_x, child_y, provider, consumer_vector, transmitter_vector)

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

if __name__ == "__main__":
    # 输入部分
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
        map.node_matrix[consumer.x][consumer.y].type_id = i
    controller = Controller(provider, map)

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
        