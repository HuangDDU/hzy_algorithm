from queue import Queue
from main7_map import Provider, Transmitter, Consumer, Node

class Map():
    def __init__(self, G_matrix) -> None:
        self.N = len(G_matrix)
        self.node_matrix = []
        for i in range(self.N):
            tmp_v = []
            for j in range(self.N):
                tmp_v.append(Node(i, j, G_matrix[i][j]))
            self.node_matrix.append(tmp_v)
    
    # 所有的Consumer都当作障碍，这里面的遍历顺序时上右下左
    def get_neighbor_list(self, x, y):
        neighbor_list = []
        if x > 0 and not (self.node_matrix[x-1][y].node_type=="CONSUMER"):
            neighbor_list.append([[x-1, y], "DOWN"]) # 当前结点在邻居结点的位置
        if y < self.N-1 and not (self.node_matrix[x][y+1].node_type=="CONSUMER"):
            neighbor_list.append([[x, y+1], "LEFT"])
        if x < self.N-1 and not (self.node_matrix[x+1][y].node_type=="CONSUMER"):
            neighbor_list.append([[x+1, y], "UP"])
        if y > 0 and not (self.node_matrix[x][y-1].node_type=="CONSUMER"):
            neighbor_list.append([[x, y-1], "RIGHT"])
        return neighbor_list
        

class Controller():
    def __init__(self, map, provider, consumer_vector) -> None:
        pass

    # BFS整张图，所有的Customer当作障碍
    def bfs(self, map, provider):
        # 队列初始化
        q = Queue()
        q.put((provider.x, provider.y))
        provider_node = map.node_matrix[provider.x][provider.y]
        provider_node.visited = True
        provider_node.distance = 0
        provider_node.best_direction = "UNKOWN"

        while not q.empty():
            current_x, current_y = q.get()
            current_node = map.node_matrix[current_x][current_y]
            for (neighbor_x, neighbor_y), direction in map.get_neighbor_list(current_x, current_y):
                # 遍历邻居，不考虑代价
                neighbor_node = map.node_matrix[neighbor_x][neighbor_y]
                if not neighbor_node.visited:
                    q.put((neighbor_x, neighbor_y))
                    neighbor_node.visited = True
                    neighbor_node.distance = current_node.distance + 1
                    neighbor_node.best_direction = direction
    
    # 刷新所有的Consumer结点
    def refresh_consumer(self, map, consumer_vector):
        reverse_direction = {
            "DOWN": "UP",
            "LEFT": "RIGHT",
            "UP": "DOWN",
            "RIGHT": "LEFT",
        }
        for consumer in consumer_vector:
            consumer_node = map.node_matrix[consumer.x][consumer.y]
            consumer_node.visited = True
            # 搜索其邻居找到遍历层数最浅的
            neighbor_list = map.get_neighbor_list(consumer.x, consumer.y)
            min_distance_neighbor_node = map.node_matrix[neighbor_list[0][0][0]][neighbor_list[0][0][1]]
            min_distance_neighbor_direction =  neighbor_list[0][1]
            consumer_node.distance =  min_distance_neighbor_node.distance + 1
            for (neighbor_x, neighbor_y), direction in neighbor_list[1:]:
                neighbor_node = map.node_matrix[neighbor_x][neighbor_y]
                if neighbor_node.distance < min_distance_neighbor_node.distance:
                    min_distance_neighbor_node = neighbor_node
                    min_distance_neighbor_direction = direction
            consumer_node.distance =  min_distance_neighbor_node.distance + 1
            consumer_node.best_direction = reverse_direction[min_distance_neighbor_direction] # 这里以consumer为中心，需要反向
            print(consumer.id, consumer_node.distance)

    # 获得所有所有Consumer到Provier的最短路径
    def get_all_trajectory(self, map, provider, consumer_vector):
        print("=======================================")
        self.bfs(map, provider)
        self.refresh_consumer(map, consumer_vector)
            

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
            node.node_type = "TRANSMITTER"
            node.type_id = transmitter.id
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
                node.node_type = "TRANSMITTER"
                node.type_id = transmitter.id
                last.target_vector.append([0, transmitter.id, 0])
                transmitter_vector.append(transmitter)
                self.dfs(map, child_x, child_y, transmitter, consumer_vector, transmitter_vector)
        else:
            # 叶子节点即遇到了Consumer
            consumer = consumer_vector[node.type_id-1]
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
    controller = Controller(map, provider, consumer_vector)

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
    
    return map, provider, consumer_vector, transmitter_vector


if __name__ == "__main__":
    # 输入部分
    # main()
    main(conf_file="main7_map_input.txt")
