import sys
import random
import numpy as np
from queue import Queue, PriorityQueue

# 输出调试日志
import logging
logger = logging.getLogger()
# 文件保存
file_handler = logging.FileHandler("main.log", "w", encoding="utf-8")
logger.addHandler(file_handler)
# 日志级别
logger.setLevel(logging.DEBUG)

# 常量参数
n = 200
robot_num = 10
berth_num = 10
N = 210
MAX_DISTANCE = 40000 # 路径的最大长度，跑遍整张地图路径
MAX_FRAME = 15000 # 最大帧数

# 方向与坐标变化的映射关系
STOP = -1
RIGHT = 0
LEFT = 1
UP = 2
DOWN = 3
direction2pos_change ={
    STOP : (0, 0),
    RIGHT : (0, 1),
    LEFT : (0, -1),
    UP : (-1, 0),
    DOWN : (1, 0),
}
pos_change2dirction = {
    (0, 0) : STOP,
    (0, 1) : RIGHT,
    (0, -1) : LEFT,
    (-1, 0) : UP,
    (1, 0) : DOWN,
}
# 反向字典
rev_direction_dict = {
    RIGHT : LEFT,
    LEFT : RIGHT,
    UP : DOWN,
    DOWN : UP
}

# 接受输入的实体类部分
# 机器人
class Robot:
    def __init__(self, id, startX=0, startY=0, goods=0, status=0, mbx=0, mby=0):
        self.id = id
        self.x = startX
        self.y = startY
        self.goods = goods
        self.status = status
        self.mbx = mbx
        self.mby = mby

        self.available = True # 0号泊位可达性
        self.available_list = [ True for i in range(berth_num)] # 到各个泊位的可达性

        self.aim_type = "" # 目标为空""，货物"good", 泊位"berth", 碰撞后恢复空地"space"
        self.aim_type_raw = "" # 碰撞后用来保存当前目标类型
        self.aim_pos = (-1, -1) # 目标位置
        self.aim_pos_raw = (-1, -1) # 碰撞后用来保存当前目标
        self.aim_berth_id = -1 # 如果目标是泊位，需要记住这个泊位id，方便往装载队列里添加
        self.direction_list = []
        self.direction = STOP

        self.pos_direction_dict ={}

        self.berth_id = -1 # 绑定机器人和坐标的关系

    def get_next_pos(self):
        pos_change = direction2pos_change[self.direction]
        next_pos = (self.x+pos_change[0], self.y+pos_change[1])
        return next_pos
    
    # 更新方向
    def update_direction(self):
        if (self.x,self.y) in self.pos_direction_dict.keys():
            self.direction = self.pos_direction_dict[(self.x, self.y)]
        else:
            self.direction = STOP

    # 设置新目标并保存之前目标
    def set_backup_aim(self, new_aim_pos, new_aim_type="space"):
        self.aim_pos_raw = self.aim_pos
        self.aim_type_raw = self.aim_type
        self.aim_pos = new_aim_pos
        self.aim_type = new_aim_type

    # 恢复目标
    def recover_aim(self):
        self.aim_pos = self.aim_pos_raw
        self.aim_type = self.aim_type_raw
        logger.debug("recover successsful!")

robot = [Robot(i) for i in range(robot_num)]


# 泊位
class Berth:
    def __init__(self, id, x=0, y=0, transport_time=0, loading_speed=0):
        self.id = id
        self.x = x
        self.y = y
        self.transport_time = transport_time
        self.loading_speed = loading_speed
        self.boat_id = -1 # 停靠的轮船id，没有停靠则为-1
        self.boat_id_raw = -1 # 划分时指定的boat
        # self.new_good_stack = [] # 归属于该泊位的货物
        self.new_good_queue = PriorityQueue() # 结点归属于该泊位的货物优先级队列，用性价比排序
        self.good_queue = Queue() #  已到达泊位的货物队列
        self.loaded_good_num = 0 #  已经往当前船上装载的货物数量
        
        self.robot_distance_list = [MAX_DISTANCE for i in range(robot_num)]

berth = [Berth(i) for i in range(berth_num)]


# 轮船
class Boat:
    def __init__(self, id, num=0, pos=0, status=0):
        self.id = id
        self.num = num
        self.pos = pos
        self.status = status
        self.berth_id = -1


boat = [Boat(i) for i in range(10)]


class Good:
    def __init__(self, x, y, value, distance, t):
        self.x = x
        self.y = y
        self.value = value
        self.distance = distance # 距离目标泊位的距离
        self.t = t # 出现时间
        self.priority = value/distance  # 希望价格尽可能大而距离尽可能小, 优先级越高
    
    def __str__(self):
        return f"f{(self.x, self.y)} | distance : {self.distance} | priority : {self.priority}({self.value}/{self.distance}| t : {self.t})"
    
    def __lt__(self, other):
        return self.priority > other.priority # 优先级越高，越在队列之前


# 地图中的结点
robot_id_count = 0
class Node:
    def __init__(self, x, y, type):
        self.x, self.y = x, y
        self.type = type # 海洋*，陆地.，泊位B，障碍#
        if self.type == 'A':
            self.type = '.'
            global robot_id_count
            robot[robot_id_count].x = x
            robot[robot_id_count].y = y
            robot_id_count += 1
        self.type_raw = self.type # 原始类型，可能避障更改

        # 后续用于机器人寻路，只对陆地有效
        self.berth_visited_list = [False for _ in range(berth_num)] # 是否有到各个泊位的路径
        self.berth_best_direction_list = [STOP for _ in range(berth_num)] # 到该泊位路径的最短方向
        self.berth_distance_list= [MAX_DISTANCE for _ in range(berth_num)] # 到该泊位路径的长度
        self.berth_best_pos_direction_dict = [{} for _ in range(berth_num)] # 回溯得到的路径
        self.nearset_berth_pos_list = [(-1, -1) for _ in range(berth_num)]# 标记最近的berth的坐标
    
        self.berth_id = -1 # 归属泊位号



class Map:
    def __init__(self):
        self.node_matrix = []
    
    def init_map(self, ch):
        for x in range(n):
            tmp_node_list = []
            for y in range(n):
                item = ch[x][0][y]
                tmp_node_list.append(Node(x, y, item))
            self.node_matrix.append(tmp_node_list)
    
    def get_neighbor_list(self, x, y):
        neighbor_list = []
        if x > 0:
            neighbor_list.append([[x-1, y], DOWN])
        if x < n-1:
            neighbor_list.append([[x+1, y], UP])
        if y > 0:
            neighbor_list.append([[x, y-1], RIGHT])
        if y < n-1:
            neighbor_list.append([[x, y+1], LEFT])
        return neighbor_list

    
    # 从固定泊位开始的bfs
    def bfs_berth(self, berth):
        berth_id = berth.id
        berth_x, berth_y = berth.x, berth.y
        # 队列初始化
        q = Queue()
        # 对上下左右边上四周的16个点初始化，只对陆地点.设置
        if berth_x > 0:
            for y in range(berth_y, berth_y+4):
                node = self.node_matrix[berth_x-1][y]
                if node.type == '.':
                    q.put((node.x, node.y))
                    node.berth_visited_list[berth_id] = True
                    node.berth_best_direction_list[berth_id] = DOWN
                    node.berth_distance_list[berth_id] = 1
                    node.berth_best_pos_direction_dict[berth_id][(node.x, node.y)] = DOWN
                    node.nearset_berth_pos_list[berth_id] = (berth_x, y)
                    # logger.debug(f"{node.x}, {node.y}, DOWN")
                    # logger.debug(f"nearset_berth_pos : {node.nearset_berth_pos_list[berth_id]}")
        if berth_x+4 < n:
            for y in range(berth_y, berth_y+4):
                node = self.node_matrix[berth_x+4][y]
                if node.type == '.':
                    q.put((node.x, node.y, ))
                    node.berth_visited_list[berth_id] = True
                    node.berth_best_direction_list[berth_id] = UP
                    node.berth_distance_list[berth_id] = 1
                    node.berth_best_pos_direction_dict[berth_id][(node.x, node.y)] = UP
                    node.nearset_berth_pos_list[berth_id] = (berth_x+3, y)
                    # logger.debug(f"{node.x}, {node.y}, UP")
                    # logger.debug(f"nearset_berth_pos : {node.nearset_berth_pos_list[berth_id]}")
        if berth_y > 0:
            for x in range(berth_x, berth_x+4):
                node = self.node_matrix[x][berth_y-1]
                if node.type == '.':
                    q.put((node.x, node.y))
                    node.berth_visited_list[berth_id] = True
                    node.berth_best_direction_list[berth_id] = RIGHT
                    node.berth_distance_list[berth_id] = 1
                    node.berth_best_pos_direction_dict[berth_id][(node.x, node.y)] = RIGHT
                    node.nearset_berth_pos_list[berth_id] = (x, berth_y)
                    # logger.debug(f"{node.x}, {node.y}, RIGHT")
                    # logger.debug(f"nearset_berth_pos : {node.nearset_berth_pos_list[berth_id]}")
        if berth_y+4 < n:
            for x in range(berth_x, berth_x+4):
                node = self.node_matrix[x][berth_y+4]
                if node.type == '.':
                    q.put((node.x, node.y))
                    node.berth_visited_list[berth_id] = True
                    node.berth_best_direction_list[berth_id] = LEFT
                    node.berth_distance_list[berth_id] = 1
                    node.berth_best_pos_direction_dict[berth_id][(node.x, node.y)] = LEFT
                    node.nearset_berth_pos_list[berth_id] = (x, berth_y+3)
                    # logger.debug(f"{node.x}, {node.y}, LEFT")
                    # logger.debug(f"nearset_berth_pos : {node.nearset_berth_pos_list[berth_id]}")
        # 深度优先遍历
        while not q.empty():
            x, y = q.get()
            node = self.node_matrix[x][y]
            for (neighbor_x, neighbor_y), direction in self.get_neighbor_list(x, y):
                neighbor_node = self.node_matrix[neighbor_x][neighbor_y]
                if neighbor_node.type == '.' and neighbor_node.berth_visited_list[berth_id]==False:
                    neighbor_node.nearset_berth_pos_list[berth_id] = node.nearset_berth_pos_list[berth_id] # 最近berth点不变
                    # logger.debug(f"x={x}, y={y}, nearset_berth_pos{neighbor_node.nearset_berth_pos_list[berth_id]}")
                    neighbor_node.berth_distance_list[berth_id] = node.berth_distance_list[berth_id] + 1
                    # 对没有访问过的陆地点设置
                    neighbor_node.berth_visited_list[berth_id] = True
                    neighbor_node.berth_best_direction_list[berth_id] = direction
                    pos_direction_dict = node.berth_best_pos_direction_dict[berth_id].copy() # 先保存前驱结点的路径
                    pos_direction_dict[(neighbor_x, neighbor_y)] = direction # 再添加前驱的方向
                    # logger.debug(pos_direction_dict)
                    neighbor_node.berth_best_pos_direction_dict[berth_id] = pos_direction_dict
                    q.put((neighbor_x, neighbor_y))
        
        # # 输出可及性
        # for i in range(n):
        #     row = ""
        #     for j in range(n):
        #         node = m.node_matrix[i][j]
        #         if node.type == '.':
        #             direction = node.berth_best_direction_list[berth_id]
        #             if direction == UP:
        #                 row += '↑'
        #             elif direction == DOWN:
        #                 row += '↓'
        #             elif direction == LEFT:
        #                 row += '←'
        #             elif direction == RIGHT:
        #                 row += '→'
        #             else:
        #                 row += 'S'
        #         else:
        #             row += node.type
        #     logger.debug(row)
    
    # 机器人位置与目标位置的A*算法
    def heuristic_distance(self, start, aim):
        return abs(start[0] - aim[0]) + abs(start[1] - aim[1])
    
    # 机器人位置与目标位置的A*算法
    def pos_A_star(self, start, aim):
        logger.debug(f"A*: ({start})->({aim})")
        q = PriorityQueue()
        q.put((0, start))
        came_from = {}
        cost_so_far = {}
        came_from[start] = None
        cost_so_far[start] = 0

        while not q.empty():
            current= q.get()[1]
            # logger.debug(f"current: {current}")

            if current == aim:
                break
            
            neighbor_list = self.get_neighbor_list(current[0], current[1])
            # logger.debug(f"neighbor_list: {neighbor_list}")

            for neighbor, direction in neighbor_list:
                node = self.node_matrix[neighbor[0]][neighbor[1]]
                if node.type == '.' or node.type == 'B': 
                    # TODO: 初赛暂时可以在B上跑
                    neighbor = tuple(neighbor) # 转化为元组才可用
                    # logger.debug(f"neighbor: {neighbor}")
                    new_cost = cost_so_far[current] + 1 # 网格类型数据代价都是1
                    # logger.debug(f"new_cost: {new_cost}")
                    if (not neighbor in cost_so_far.keys()) or (new_cost < cost_so_far[neighbor]):
                        cost_so_far[neighbor] = new_cost
                        priority = new_cost + self.heuristic_distance(neighbor, aim)
                        # logger.debug(f"priority: {priority}")
                        q.put((priority, neighbor))
                        came_from[neighbor] = current
            
        # logger.debug(f"came_from : {came_from}")
        if not aim in came_from.keys():
            # 不可达
            # logger.debug(f"A* can't achieve: ({start})->({aim})")
            return {}


        # 回溯
        tmp_pos = aim
        pos_list = []
        direction_list = []

        while not tmp_pos == start:
            pre_pos = came_from[tmp_pos]
            pos_list.append(pre_pos)
            dirction = pos_change2dirction[(tmp_pos[0]-pre_pos[0], tmp_pos[1]-pre_pos[1])]
            direction_list.append(dirction)
            tmp_pos = pre_pos
        # logger.debug(f"pos_list : {pos_list}")
        # logger.debug(f"direction_list : {direction_list}")
        
        # # 输出路径
        pos_direction_dict = dict(zip(pos_list, direction_list))
        
        return pos_direction_dict


    # 测试该位置是否为空地
    def test_space(self, x, y):
        if not ((x > 0) and (x < n) and (y > 0) and (y < n)):
            return False
        node = self.node_matrix[x][y]
        if not node.type == '.':
            return False
        # 8个方向的邻居单元必须为陆地
        if not (((x-1) > 0) and ((y-1) > 0) and self.node_matrix[x-1][y-1].type == '.'):
            return False
        if not (((x-1) > 0) and self.node_matrix[x-1][y].type == '.'):
            return False
        if not (((x-1) > 0) and ((y+1) < n) and self.node_matrix[x-1][y+1].type == '.'):
            return False
        if not (((y-1) > 0) and self.node_matrix[x][y-1].type == '.'):
            return False
        if not (((y+1) < n) and self.node_matrix[x][y+1].type == '.'):
            return False
        if not (((x+1) < n) and ((y-1) > 0) and self.node_matrix[x+1][y-1].type == '.'):
            return False
        if not (((x+1) < n) and self.node_matrix[x+1][y].type == '.'):
            return False
        if not (((x+1) < n) and ((y+1) < n) and self.node_matrix[x+1][y+1].type == '.'):
            return False
        return True

    # 查找离当前位置欧式距离最近的空地
    def search_space(self, x, y):
        for i in range(1, n):
            for j in range(i+1):
                test_x, test_y = x + i, y + j
                if self.test_space(test_x, test_y):
                    return test_x, test_y
                test_x, test_y = x + i, y - j
                if self.test_space(test_x, test_y):
                    return test_x, test_y
                test_x, test_y = x - i, y + j
                if self.test_space(test_x, test_y):
                    return test_x, test_y
                test_x, test_y = x - i, y - j
                if self.test_space(test_x, test_y):
                    return test_x, test_y
        return (-1, -1)
                
m = Map()

money = 0
boat_capacity = 0
id = 0
ch = []
gds = [[0 for _ in range(N)] for _ in range(N)] # 货物列表
# TODO: 划分机器人与泊位的归属关系，暂时手动指定，1对多
# 顺序指定
berth2robot = {
    0: [0, 1],
    1: [2, 3],
    2: [4, 5],
    3: [6, 7],
    4: [8, 9],
}


berth_id_list = []
def get_distance(xi, yi, xj, yj):
    return abs(xi-xj)+abs(yi-yj)

# 默认0号泊位在候选泊位中，并选择其他4个泊位
def get_berth_id_list(berth_pos_list, step=4, selected_list=[0], to_be_selected_list=list(range(0, 10))):

    for k in range(step):
        # 从候选集中选择与已选的所有泊位都远的
        # print(selected_list)
        min_distance_list = []
        to_be_selected_list = list(to_be_selected_list) # 需要定位索引
        for i in to_be_selected_list:
            # 取到达已选泊位距离最近的距离值作为候选泊位的距离值
            distance_list = []
            for j in selected_list:
                xi, yi = berth_pos_list[i]
                xj, yj = berth_pos_list[j]
                distance = get_distance(xi, yi, xj, yj)
                distance_list.append(distance)
            min_distance = min(distance_list)
            min_distance_list.append(min_distance)
        # print(min_distance_list)
        # 距离候选泊位距离值中，选择距离最大的泊位
        max_min_distance = max(min_distance_list)
        index = min_distance_list.index(max_min_distance)
        selected_id = to_be_selected_list[index]
        to_be_selected_list.remove(selected_id)
        selected_list.append(selected_id)
    
    # print(selected_list)

    return selected_list


# 使用轮询，为泊位分配机器人
def get_berth2robot(berth_id_list, distance_list_list):
    avalilable_robot_num = 10
    # 先剔除掉不可用的机器人
    for i in range(10):
        flag = False # 该机器人是否可用，只要能到一个berth即可用
        for j in range(5):
            if distance_list_list[j][i] < MAX_DISTANCE:
                flag = True
                break
        if not flag:
            robot[i].available = False
            logger.debug(f"robot({j}) is unavailable")
            avalilable_robot_num -= 1
    logger.debug(f"avalilable robot num {avalilable_robot_num}")
    
    # 创建队列
    distance_priority_queue_list = []
    for distance_list in distance_list_list:
        q = PriorityQueue()
        for i in range(len(distance_list)):
            distance = distance_list[i]
            if not distance == MAX_DISTANCE:
                q.put((distance, i)) # 用距离排序，并保存robot ID
        distance_priority_queue_list.append(q)
        # print(q.queue)

    # 开始轮询，分配机器人给泊位
    robot_list_list = [[] for i in range(5)]
    selected_set = set()
    while not len(selected_set) == avalilable_robot_num:
        for i in range(5):
            q = distance_priority_queue_list[i]
            while not q.empty():
                robot_id = q.get()[1]
                if robot_id not in selected_set:
                    # 本次轮询选中了1个robot
                    selected_set.add(robot_id)
                    robot_list_list[i].append(robot_id)
                    # print(i, robot_id)
                    break

    berth2robot = dict(zip(berth_id_list, robot_list_list))
    return berth2robot

# 接受初始输入
def Init():
    # 地图
    for i in range(0, n):
        line = input()
        ch.append([c for c in line.split(sep=" ")]) # 
    # 泊位
    berth_pos_list = []
    for i in range(berth_num):
        line = input()
        berth_list = [int(c) for c in line.split(sep=" ")]
        id = berth_list[0]
        berth[id].x = berth_list[1]
        berth[id].y = berth_list[2]
        berth_pos_list.append((berth[id].x, berth[id].y))
        berth[id].transport_time = berth_list[3]
        berth[id].loading_speed = berth_list[4]
    global boat_capacity # 全局变量的声明
    boat_capacity = int(input())
    # 地图初始化
    m.init_map(ch)
    # TODO: 计算侯选泊位
    global berth_id_list
    berth_id_list = get_berth_id_list(berth_pos_list)
    logger.debug(f"selected berth_id_list: {berth_id_list}")
    # TODO: 计算所有陆地点到5个泊位的最短路径，划分归属的泊位
    distance_list_list = []
    for i in berth_id_list:
        berth_i = berth[i]
        logger.debug(f"bfs berth {i}......")
        m.bfs_berth(berth_i)
        logger.debug("bfs end......")
        logger.debug("check robot availability......")
        # 机器人距离设置
        for j in range(10):
            robot_j = robot[j]
            node = m.node_matrix[robot_j.x][robot_j.y]
            if node.berth_visited_list[i]:
                berth_i.robot_distance_list[j] = node.berth_distance_list[i]
        distance_list_list.append(berth_i.robot_distance_list)
    logger.debug(f"distance_list_list : {distance_list_list}")
    # TODO: 使用轮询计算泊位对应的robot
    berth2robot = get_berth2robot(berth_id_list, distance_list_list)
    logger.debug(f"berth2robot : {berth2robot}")
    # 泊位与robot、boat的绑定
    boat_id_count = 0
    for berth_id, robot_id_list in berth2robot.items():
        berth_item = berth[berth_id]
        berth_item.robot_id_list = robot_id_list
        boat[boat_id_count].berth_id = berth_id
        boat_id_count += 1
        logger.debug(f"boat({boat_id_count}):berth({berth_id})")
        berth_item.robot_id_list = robot_id_list
        for robot_id in robot_id_list:
            robot_item = robot[robot_id]
            robot_item.berth_id = berth_id
    # 如果有泊位完全孤立，没有机器人可以到达该泊位，则重新替换该泊位
    # if len(berth_id_list) < 5:
    #     logger.debug("重新选择泊位")
    # 划分各个地图结点归属的泊位
    logger.debug("split node to berth")
    for i in range(n):
        row = "" # 用于输出的行
        for j in range(n):
            node = m.node_matrix[i][j]
            if node.type == '.':
                distance_list = node.berth_distance_list
                distance_list.index(min(distance_list))
                node.berth_id = distance_list.index(min(distance_list)) # 改换距离为最大值之后，可以直接查找
                row += str(node.berth_id)
            else:
                row += node.type
        logger.debug(row)

    # OK确定
    okk = input()
    print("OK")
    sys.stdout.flush()

# 接受单帧输入
def Input():
    global id, money
    id, money = map(int, input().split(" ")) # frame_id，收益
    logger.debug(f"\n=====frame:{id}, money:{money}, INPUT=====")
    # 新增物品部分
    num = int(input())
    for i in range(num):
        x, y, val = map(int, input().split())
        gds[x][y] = val
        # 结点归属的泊位，添加到货物队列里
        # berth_id = m.node_matrix[x][y].berth_id
        node = m.node_matrix[x][y]
        berth_id = node.berth_id
        if not berth_id == -1:
            # berth[berth_id].new_good_stack.append((x, y))
            # TODO: 改换成Good对象，使用优先级队列，按照性价比排序
            gd_object = Good(x, y, val, node.berth_distance_list[berth_id], id)
            berth[berth_id].new_good_queue.put(gd_object)
            logger.debug(f"berth_id : {(berth_id)}, new_good : {(x, y)}")
    # 所有机器人，是否带货、坐标x、坐标y、是否碰撞后恢复
    for i in range(robot_num):
        robot[i].goods, robot[i].x, robot[i].y, robot[i].status = map(int, input().split())
    # 所有轮船，状态、目标泊位
    for i in range(5):
        boat[i].status, boat[i].pos = map(int, input().split())
    okk = input()

flag = False # 第0个机器人把第0个货物运达第0个泊位的标识符

# 单帧输出
def Output():
    logger.debug(f"=====frame:{id}, money:{money}, OUTPUT=====")

    # 主动检测碰撞
    logger.debug("start robot conclision search")
    for i in range(10):
        robot_i = robot[i]
        robot_i.update_direction()
        # 检测比自己id大的机器人，是否碰撞
        for j in range(i+1, 10):
            robot_j = robot[j]
            if (robot_i.available == False) or (robot_j.available == False):
                continue
            robot_j.update_direction()
            robot_i_pos = (robot_i.x, robot_i.y)
            robot_i_next_pos = robot_i.get_next_pos()
            robot_j_pos = (robot_j.x, robot_j.y)
            robot_j_next_pos = robot_j.get_next_pos()
            colision_flag = False
            colision_type = -1
            if robot_i_next_pos == robot_j_next_pos:
                # 间隔型碰撞 
                colision_flag = True
                colision_type = 0
                logger.debug(f"robot ({i}[{robot_i_pos}]),({j}[{robot_j_pos}]) will colision type 0")
            elif (robot_i_next_pos == robot_j_pos) and (robot_j_next_pos == robot_i_pos) :
                # 紧贴型碰撞
                colision_flag = True
                colision_type = 1
                logger.debug(f"robot ({i}[{robot_i_pos}]),({j}[{robot_j_pos}]) will colision type 1")
            
            if colision_flag:
                # id越小优先级越高
                robot_higher = robot_i
                robot_lower = robot_j
                # 高优先级的暂停
                logger.debug(f"robot ({robot_higher.id}) higher, wait")
                # 低优先级重新规划路径
                new_obscale_pos = [-1, -1]
                if colision_type == 0:
                    # 间隔型碰撞
                    robot_lower_next_pos = robot_lower.get_next_pos()
                    new_obscale_pos[0] = robot_lower_next_pos[0]
                    new_obscale_pos[1] = robot_lower_next_pos[1]
                else:
                    new_obscale_pos[0] = robot_higher.x
                    new_obscale_pos[1] = robot_higher.y
                logger.debug(f"new_obscale_pos : {new_obscale_pos}")
                new_obscale_node = m.node_matrix[new_obscale_pos[0]][new_obscale_pos[1]]
                new_obscale_node.type = '#'
                pos_direction_dict = m.pos_A_star((robot_lower.x, robot_lower.y), robot_lower.aim_pos) 
                if len(pos_direction_dict) == 0:
                    # A*不可达,先等等找一处空地
                    logger.debug(f"A* can't achieve: ({(robot_lower.x, robot_lower.y)})->robot_lower.aim_pos")

                    space_aim_pos = (-1, -1) # 搜寻最近的空地
                    space_aim_pos = m.search_space(robot_lower.x, robot_lower.y)
                    logger.debug(f"robot ({robot_lower.id}) try to go to space {space_aim_pos}")
                    robot_lower.set_backup_aim(space_aim_pos)
                    pos_direction_dict = m.pos_A_star((robot_lower.x, robot_lower.y), space_aim_pos) 
                robot_lower.pos_direction_dict = pos_direction_dict
                logger.debug(f"robot ({robot_lower.id}) lower, replan pos_direction_dict (A*) : {robot_lower.pos_direction_dict}")
                
                new_obscale_node.type = new_obscale_node.type_raw
    logger.debug("finish robot conclision search")
    
    # TODO: 所有机器人调度
    for i in range(10):
        robot_i = robot[i]
        if robot_i.available == False:
            # 不可用的机器人跳过
            continue

        if robot_i.status == 0:
            # 这里被迫碰撞
            logger.debug(f"robot({i}) status concolision")
        
        else:
            if robot_i.aim_type == "":
                # 没有带货，规划到最新的可达货物的路径
                logger.debug(f"robot({i}) plan start")
                
                berth_item = berth[robot_i.berth_id]

                # TODO: 修改，从优先级队列中取货
                new_good_queue = berth_item.new_good_queue
                new_good = None
                while not new_good_queue.empty():
                    new_good = new_good_queue.get()
                    period = 1000 - (id - new_good.t)
                    logger.debug(f"good {(new_good.x, new_good.y)}: distance:{new_good.distance}, period: {period}")
                    if new_good.distance <= 1000 - (id - new_good.t):
                        # 此时没有考虑，碰撞，直接认为不会超时
                        logger.debug(f"robot({i}) have available good({new_good}) for berth ({berth_item.id})")
                        # 暂时主动寻路
                        # robot_i.pos_direction_dict = m.pos_A_star((robot_i.x, robot_i.y), (new_good.x, new_good.y))
                        pos_direction_dict = {}
                        
                        # TODO: 以后可能还需要复用bfs寻路
                        robot_node = m.node_matrix[robot_i.x][robot_i.y]
                        if robot_node.type == 'B':
                            good_node = m.node_matrix[new_good.x][new_good.y]
                            # 在泊位，则原本BFS得到的路径逆转+Berth局部A*
                            bfs_pos_direction_dict = good_node.berth_best_pos_direction_dict[good_node.berth_id]
                            # logger.debug(f"bfs_pos_direction_dict(bfs) : {bfs_pos_direction_dict}")
                            reversed_bfs_pos_direction_dict =  {} # 原本BFS得到的路径逆转
                            for pos, direction in bfs_pos_direction_dict.items():
                                rev_rection = rev_direction_dict[direction]
                                pos_change = direction2pos_change[direction]
                                next_pos = (pos[0]+pos_change[0], pos[1]+pos_change[1])
                                reversed_bfs_pos_direction_dict[next_pos] = rev_rection
                            logger.debug(f"pos_direction_dict(bfs reverse) : {reversed_bfs_pos_direction_dict}")
                            nearset_berth_pos = good_node.nearset_berth_pos_list[good_node.berth_id] # 标记最近的berth的坐标，作为Berth局部A*的终止点
                            logger.debug(f"nearset_berth_pos : {nearset_berth_pos}")
                            pos_direction_dict =  m.pos_A_star((robot_i.x, robot_i.y), nearset_berth_pos) # Berth局部A*，5步只能能完成
                            logger.debug(f"pos_direction_dict(berth local) : {pos_direction_dict}")
                            pos_direction_dict.update(reversed_bfs_pos_direction_dict)
                            logger.debug(f"pos_direction_dict(merge) : {pos_direction_dict}") # 路径拼接

                        else:
                            # 主动寻路，整体A*
                            pos_direction_dict = m.pos_A_star((robot_i.x, robot_i.y), (new_good.x, new_good.y))
                            logger.debug(f"pos_direction_dict (A*) : {pos_direction_dict}")
                        robot_i.pos_direction_dict = pos_direction_dict
                        logger.debug(f"plan pos_direction_dict : {robot_i.pos_direction_dict}")
                        robot_i.aim_type = "good"
                        robot_i.aim_pos = (new_good.x, new_good.y)
                        break # 有货了就跳出去

                    else:
                        # 到达该结点可能会超时，直接丢弃
                        logger.debug(f"robot({i}) test good({new_good}) for berth ({berth_item.id}) : over time")
                if new_good == None:
                    logger.debug(f"robot({i}) have no available good for berth ({berth_item.id})")
                
                logger.debug(f"robot({i})plan end")

            elif robot_i.aim_type == "good":
                # 移动到货物的位置
                robot_pos = (robot_i.x, robot_i.y)
                logger.debug(f"robot({i})|(good) : {robot_pos}->{robot_i.aim_pos}")
                if not robot_i.aim_pos == robot_pos:
                    robot_i.update_direction()
                    direction = robot_i.direction
                    print(f"move {i} {direction}")
                else:
                    print(f"get {i}")
                    logger.debug(f"robot({i})货物get成功")
                    robot_i.aim_type = "berth"
                    # TODO: 获取原本BFS回溯的路径作为初始路径
                    node = m.node_matrix[robot_i.x][robot_i.y]
                    # robot_i.pos_direction_dict = node.berth_best_pos_direction_dict[0] # 初始按照BFS走
                    # robot_i.aim_pos = (berth[0].x, berth[0].y)
                    aim_berth = berth[node.berth_id] # 选择当前目标berth
                    robot_i.pos_direction_dict = node.berth_best_pos_direction_dict[aim_berth.id] # 
                    robot_i.aim_pos = (aim_berth.x, aim_berth.y)
                    robot_i.aim_berth_id =aim_berth.id
                    logger.debug(f"pos_direction_dict(bfs) : {robot_i.pos_direction_dict}")
            elif robot_i.aim_type == "berth":
                # 带货了，移动到特定泊位
                robot_pos = (robot_i.x, robot_i.y)
                logger.debug(f"robot({i})|(berth) : {robot_pos}->{robot_i.aim_pos}")
                node = m.node_matrix[robot_pos[0]][robot_pos[1]]
                if node.type == 'B':
                    # 到了泊位就放下
                    print(f"pull {i}")
                    logger.debug(f"robot({i})货物pull成功")
                    robot_i.aim_type = ""
                    berth[robot_i.aim_berth_id].good_queue.put(0) # 随便放一个元素，只是占位置用的
                else:
                    robot_i.update_direction()
                    direction = robot_i.direction
                    print(f"move {i} {direction}")
            elif robot_i.aim_type == "space":
                # 解决冲突,移动到空地上
                robot_pos = (robot_i.x, robot_i.y)
                logger.debug(f"robot({i})|(space) : {robot_pos}->{robot_i.aim_pos}")
                node = m.node_matrix[robot_pos[0]][robot_pos[1]]
                if not robot_i.aim_pos == robot_pos:
                    # 正在往空地移动
                    robot_i.update_direction()
                    direction = robot_i.direction
                    print(f"move {i} {direction}")
                else:
                    # 移动到空地,恢复目标
                    logger.debug(f"robot({i}) recover aim: {robot_i.aim_pos_raw}")
                    robot_i.recover_aim()
                    robot_i.pos_direction_dict = m.pos_A_star((robot_i.x, robot_i.y), robot_i.aim_pos)
                    logger.debug(f"pos_direction_dict(space recover) : {robot_i.pos_direction_dict}")

    # TODO: 所有轮船调度
    for i in range(5):
        boat_i = boat[i]
        if boat_i.status == 1:
            if boat_i.pos == -1:
                # 轮船到达虚拟点, 立即回到泊位
                logger.debug(f"boat({i}) reach virtual node: plan to berth({boat_i.berth_id})")
                print(f"ship {i} {boat_i.berth_id}")
            else:
                # 轮船到达泊位
                logger.debug(f"boat({i}) reached berth({boat_i.pos})")
                berth[boat_i.pos].boat_id = i
        elif boat_i.status == 2:
            logger.debug(f"boat({i}) is waiting for berth({boat_i.berth_id})")
        else:
            if boat_i.pos == -1:
                logger.debug(f"boat({i}) is going to virtual node")
            else:
                logger.debug(f"boat({i}) is going to berth({boat_i.berth_id})")

    # TODO: 所有泊位调度
    for i in berth_id_list:
        berth_i = berth[i]
        new_good_queue_str = [str(new_good) for new_good in list(berth_i.new_good_queue.queue)]
        logger.debug(f"berth({i}) range good queue {new_good_queue_str}") # 当前泊位归属的货物队列
        if not berth_i.boat_id == -1:
            if id >= MAX_FRAME - berth[i].transport_time - 5:
                # 最后快结束的时候，留出10帧的时间离开
                print(f"go {berth[i].boat_id}")
                logger.debug(f"game will over, ship {berth[i].boat_id}")
                berth_i.boat_id = -1
                berth_i.loaded_good_num = 0
                continue
            # 当前泊位有船则开始装货
            for j in range(berth_i.loading_speed):
                if berth_i.good_queue.empty():
                    break
                berth_i.good_queue.get() # 取出一个货
                berth_i.loaded_good_num += 1
                logger.debug(f"berth({i}) : {berth_i.loaded_good_num }/{boat_capacity}")
                if berth_i.loaded_good_num == boat_capacity:
                    # 货装满了，轮船出发
                    print(f"go {berth_i.boat_id}")
                    logger.debug(f"berth({i}) full, boat({berth_i.boat_id}) go!")
                    berth_i.boat_id = -1
                    berth_i.loaded_good_num = 0
                    break
    
    return

if __name__ == "__main__":
    Init()
    for zhen in range(1, 15001):
        Input()
        Output()
        for i in range(robot_num):
            # print("move", i, random.randint(0, 3))
            sys.stdout.flush()
        print("OK")

        sys.stdout.flush()
    logger.debug(f"money:{money}")