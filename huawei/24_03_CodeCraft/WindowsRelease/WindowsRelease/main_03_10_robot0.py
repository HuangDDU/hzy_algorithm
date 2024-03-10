
import sys
import random
import numpy as np
from queue import Queue, PriorityQueue

# 输出调试日志
import logging
logger = logging.getLogger()
# 文件保存
file_handler = logging.FileHandler("main_03_10_robot0.log", "w", encoding="utf-8")
logger.addHandler(file_handler)
# 格式
# formatter = logging.Formatter('%(asctime)s - %(levelname)s - %(message)s')
# file_handler.setFormatter(formatter)
# 日志级别
logger.setLevel(logging.DEBUG)

# 调试断点
import time
# time.sleep(20)

# 常量参数
n = 200
robot_num = 10
berth_num = 10
N = 210


# 接受输入的实体类部分
# 机器人
class Robot:
    def __init__(self, startX=0, startY=0, goods=0, status=0, mbx=0, mby=0):
        self.x = startX
        self.y = startY
        self.goods = goods
        self.status = status
        self.mbx = mbx
        self.mby = mby

        self.aim_type = "" # 目标为空""，货物"good", 泊位"berth"
        self.direction_list = []


robot = [Robot() for _ in range(robot_num + 10)]


# 泊位
class Berth:
    def __init__(self, x=0, y=0, transport_time=0, loading_speed=0):
        self.x = x
        self.y = y
        self.transport_time = transport_time
        self.loading_speed = loading_speed

berth = [Berth() for _ in range(berth_num + 10)]


# 轮船
class Boat:
    def __init__(self, num=0, pos=0, status=0):
        self.num = num
        self.pos = pos
        self.status = status


boat = [Boat() for _ in range(10)]

# 地图中的结点
RIGHT = 0
LEFT = 1
UP = 2
DOWN = 3
class Node:
    def __init__(self, x, y, type):
        self.x, self.y = x, y
        # self.type = "ocean" # 海洋land，陆地land，泊位berth，障碍barrier
        self.type = type # 海洋*，陆地.，泊位B，障碍#
        if self.type == 'A':
            self.item = '.'
        # 后续用于机器人寻路，只对陆地有效
        self.berth_visited_list = [False for _ in range(berth_num)] # 是否有到各个泊位的路径
        self.berth_best_direction_list = [RIGHT for _ in range(berth_num)] # 到该泊位路径的最短方向


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
    def bfs_berth(self, berth_id, berth_x, berth_y):
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
                    logger.info(f"{node.x}, {node.y}, DOWN")
        if berth_x+4 < n:
            for y in range(berth_y, berth_y+4):
                node = self.node_matrix[berth_x+4][y]
                if node.type == '.':
                    q.put((node.x, node.y, ))
                    node.berth_visited_list[berth_id] = True
                    node.berth_best_direction_list[berth_id] = UP
                    logger.info(f"{node.x}, {node.y}, UP")
        if berth_y > 0:
            for x in range(berth_x, berth_x+4):
                node = self.node_matrix[x][berth_y-1]
                if node.type == '.':
                    q.put((node.x, node.y))
                    node.berth_visited_list[berth_id] = True
                    node.berth_best_direction_list[berth_id] = RIGHT
                    logger.info(f"{node.x}, {node.y}, RIGHT")
        if berth_y+4 < n:
            for x in range(berth_x, berth_x+4):
                node = self.node_matrix[x][berth_y+4]
                if node.type == '.':
                    q.put((node.x, node.y))
                    node.berth_visited_list[berth_id] = True
                    node.berth_best_direction_list[berth_id] = LEFT
                    logger.info(f"{node.x}, {node.y}, LEFT")
        
        # 深度优先遍历
        while not q.empty():
            x, y = q.get()
            node = self.node_matrix[x][y]
            for (neighbor_x, neighbor_y), direction in self.get_neighbor_list(x, y):
                neighbor_node = self.node_matrix[neighbor_x][neighbor_y]
                if neighbor_node.type == '.' and neighbor_node.berth_visited_list[berth_id]==False:
                    # 对没有访问过的陆地点设置
                    neighbor_node.berth_visited_list[berth_id] = True
                    neighbor_node.berth_best_direction_list[berth_id] = direction
                    q.put((neighbor_x, neighbor_y))
        
    # 机器人位置与目标位置的A*算法
    def heuristic_distance(self, start, aim):
        return abs(start[0] - aim[0]) + abs(start[1] - aim[1])
    
    # 机器人位置与目标位置的A*算法
    def pos_A_star(self, start, aim):
        logger.info(f"A*: ({start})->({aim})")
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
                if node.type == '.':
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
            
        logger.debug(f"came_from : {came_from}")

        # 回溯
        tmp_pos = aim
        pos_list = []
        direction_list = []
        dirction_dict = {
            (0, 1) : RIGHT,
            (0, -1) : LEFT,
            (1, 0) : DOWN,
            (-1, 0) : UP,
        }
        while not tmp_pos == start:
            pre_pos = came_from[tmp_pos]
            pos_list.append(pre_pos)
            dirction = dirction_dict[(tmp_pos[0]-pre_pos[0], tmp_pos[1]-pre_pos[1])]
            direction_list.append(dirction)
            tmp_pos = pre_pos
        logger.info(f"pos_list : {pos_list}")
        logger.info(f"direction_list : {direction_list}")

        # 暂时不用反转了，正好弹出元素
        # direction_list = list(reversed(direction_list)) 
        
        # 输出路径
        pos_direction_dict = dict(zip(pos_list, direction_list))
        for i in range(n):
            row = ""
            for j in range(n):
                node = m.node_matrix[i][j]
                pos = (node.x, node.y)
                if pos == aim:
                    row += "👑" 
                elif pos in pos_direction_dict.keys() and node.type == '.':
                        direction = pos_direction_dict[pos]
                        if direction == UP:
                            row += '↑'
                        elif direction == DOWN:
                            row += '↓'
                        elif direction == LEFT:
                            row += '←'
                        else:
                            row += '→'
                else:
                    row += node.type
            logger.info(row)

        return direction_list


m = Map()

money = 0
boat_capacity = 0
id = 0
ch = []
gds = [[0 for _ in range(N)] for _ in range(N)] # 货物列表
gd0_pos = [-1, -1] # 第一个出现的货物坐标，保存为列表，函数才能更改其内部元素
gds_pos_statck = [] # 使用列表实现的栈

# 接受初始输入
def Init():
    # 地图
    for i in range(0, n):
        line = input()
        ch.append([c for c in line.split(sep=" ")]) # 
    # 泊位
    for i in range(berth_num):
        line = input()
        berth_list = [int(c) for c in line.split(sep=" ")]
        id = berth_list[0]
        berth[id].x = berth_list[1]
        berth[id].y = berth_list[2]
        berth[id].transport_time = berth_list[3]
        berth[id].loading_speed = berth_list[4]
    boat_capacity = int(input())
    # TODO: 计算所有陆地点到泊位的最短路径
    m.init_map(ch)
    logging.info("bfs berth 0")
    m.bfs_berth(0, berth[0].x, berth[0].y)
    # OK确定
    okk = input()
    print("OK")
    sys.stdout.flush()

# 接受单帧输入
def Input():
    id, money = map(int, input().split(" ")) # frame_id，收益
    # 新增物品部分
    num = int(input())
    for i in range(num):
        x, y, val = map(int, input().split())
        gds[x][y] = val
        gds_pos_statck.append((x, y))
        if gd0_pos[0] == -1:
            # 保存第一个货物
            gd0_pos[0] = x
            gd0_pos[1] = y
    # 所有机器人，是否带货、坐标x、坐标y、是否碰撞后恢复
    for i in range(robot_num):
        robot[i].goods, robot[i].x, robot[i].y, robot[i].status = map(int, input().split())
    # 所有轮船，状态、目标泊位
    for i in range(5):
        boat[i].status, boat[i].pos = map(int, input().split())
    okk = input()
    return id

flag = False # 第0个机器人把第0个货物运达第0个泊位的标识符

# 单帧输出
def Output(id):
    logger.debug(f"{id}")
    if id==1:
        # 初始：0号轮船到达0号泊位
        print(f"ship 0 0")
        logger.debug(f"transport_time: {berth[0].transport_time}") # 泊位到虚拟点的时间
        for i in ch:
            logger.info(f"{i}")
            # 打印所有点到0号泊位的最短路径
        for i in range(n):
            row = ""
            for j in range(n):
                node = m.node_matrix[i][j]
                if node.type == '.':
                    direction = node.berth_best_direction_list[0]
                    if direction == UP:
                        row += '↑'
                    elif direction == DOWN:
                        row += '↓'
                    elif direction == LEFT:
                        row += '←'
                    else:
                        row += '→'
                else:
                    row += node.type
            logger.info(row)

    # TODO: 0号机器人
    robot_0 = robot[0]
    if robot_0.aim_type == "" and (not len(gds_pos_statck)==0):
        # 没有带货，0号机器人对最新的货物发动规划路径请求，暂时不考虑时间
        # 取最新的货物，且要求该货物可达
        new_good = gds_pos_statck.pop()
        good_node = m.node_matrix[new_good[0]][new_good[1]]
        while not good_node.berth_visited_list[0]:
            logger.debug(f"{new_good} unavailable")
            new_good = gds_pos_statck.pop()
            good_node = m.node_matrix[new_good[0]][new_good[1]]
        direction_list = m.pos_A_star((robot_0.x, robot_0.y), new_good)
        logger.info("规划结束")
        robot_0.aim_type = "good"
        robot_0.direction_list = direction_list
    elif robot_0.aim_type == "good":
        # 移动到货物位置
        logger.debug(f"robot_0 : {robot_0.x},{robot_0.y}")
        if len(robot_0.direction_list) > 0:
            direction = robot_0.direction_list.pop()
            print(f"move 0 {direction}")
        else:
            print(f"get 0")
            logger.info(f"货物get成功")
            robot_0.aim_type = "berth"
    elif robot_0.aim_type == "berth":
        # 带货了，0号机器人移动到固定位置0号泊位
        node = m.node_matrix[robot_0.x][robot_0.y]
        direction = node.berth_best_direction_list[0]
        logger.debug(f"robot_0 : {(robot_0.x, robot_0.y)}->{(berth[0].x, berth[0].y)} | {robot_0.x},{robot_0.y}")
        # logger.info(f"({node.x},{node.y}),{direction}")
        if node.type == '.':
            print(f"move 0 {direction}")
        else:
            # 到了泊位就放下
            print("pull 0") 
            logger.info(f"货物pull成功")
            robot_0.aim_type = ""

    # TODO: 0号轮船等到最后才走压着帧数
    max_frame = 15000
    if id == max_frame - berth[0].transport_time:
        print("go 0") # 有标记了，装上货物后开往虚拟点
        logger.info(f"轮船出发")
    return

if __name__ == "__main__":
    Init()
    for zhen in range(1, 15001):
        id = Input()
        Output(id)
        for i in range(robot_num):
            # print("move", i, random.randint(0, 3))
            sys.stdout.flush()
        print("OK")

        sys.stdout.flush()
