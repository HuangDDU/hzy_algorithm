
import sys
import random
import numpy as np
from queue import Queue, PriorityQueue
from Search_2D.D_star import DStar

# 输出调试日志
import logging
logger = logging.getLogger()
logging.getLogger("matplotlib").setLevel(logging.WARNING) # 关闭其他包的日主输出
logging.getLogger("PIL").setLevel(logging.WARNING) # 关闭其他包的日主输出
# 文件保存
file_handler = logging.FileHandler("main.log", "w", encoding="utf-8")
logger.addHandler(file_handler)
# 日志级别
logger.setLevel(logging.DEBUG)
logger.debug("begin debug")

# 常量参数
n = 200
robot_num = 10
berth_num = 10
N = 210


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

        self.aim_type = "" # 目标为空""，货物"good", 泊位"berth"
        self.direction_list = []


robot = [Robot(i) for i in range(robot_num + 10)]


# 泊位
class Berth:
    def __init__(self, id, x=0, y=0, transport_time=0, loading_speed=0):
        self.id = id
        self.x = x
        self.y = y
        self.transport_time = transport_time
        self.loading_speed = loading_speed
        self.boat_id = -1 # 停靠的轮船id，没有停靠则为-1
        self.good_queue = Queue() #  货物队列
        self.loaded_good_num = 0 #  已经往当前船上装载的货物数量

berth = [Berth(i) for i in range(berth_num + 10)]


# 轮船
class Boat:
    def __init__(self, id, num=0, pos=0, status=0):
        self.id = id
        self.num = num
        self.pos = pos
        self.status = status


boat = [Boat(i) for i in range(10)]

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
            self.type = '.'
        # 后续用于机器人寻路，只对陆地有效
        self.berth_visited_list = [False for _ in range(berth_num)] # 是否有到各个泊位的路径
        self.berth_best_direction_list = [RIGHT for _ in range(berth_num)] # 到该泊位路径的最短方向

# 简单类，可以不用继承env
class NewEnv():
    def __init__(self, n, m):
        # 指定没有边界
        self.x_range = n + 2  # size of background
        self.y_range = m + 2
        self.motions = [(-1, 0), (0, 1), (1, 0), (0, -1)] # 朝四个方向走
        self.obs = self.set_obs()

    def set_obs(self, new_obs = []):
        x = self.x_range
        y = self.y_range
        obs = set()

        # 外围添加障碍
        for i in range(x):
            obs.add((i, 0))
        for i in range(x):
            obs.add((i, y - 1))

        for i in range(y):
            obs.add((0, i))
        for i in range(y):
            obs.add((x - 1, i))

        # 内部障碍
        # logger.debug(f"new_obs:{new_obs}")
        logger.debug(f"before union: {len(obs)}")
        obs = obs.union([(item[0]+1, item[1]+1) for item in new_obs])
        logger.debug(f"after union: {len(obs)}")
        return obs
    
class NewDStar(DStar):
    def __init__(self, s_start, s_goal, env):
        s_start = (s_start[0]+1, s_start[1]+1)
        s_goal = (s_goal[0]+1, s_goal[1]+1)
        super().__init__(s_start, s_goal)

        # 环境设置
        self.Env = env
        # self.Plot = NewPlotting(self.s_start, self.s_goal, env)

        self.u_set = self.Env.motions
        self.obs = self.Env.obs.copy() # 深拷贝障碍对象
        self.x = self.Env.x_range
        self.y = self.Env.y_range

    # 不用绘图的找路径函数
    def get_path(self):
        self.init()
        self.insert(self.s_goal, 0)

        while True:
            self.process_state()
            if self.t[self.s_start] == "CLOSED":
                break

        self.path = self.extract_path(self.s_start, self.s_goal)
        path = [(item[0]-1, item[1]-1) for item in self.path]
        return path 
    
    # 新添加障碍后的规划路径，新规划的起点为碰撞点
    def get_new_path(self, obscale_list):
        for item in obscale_list:
            self.obs.add((item[0]+1, item[1]+1))

        s = self.s_start
        self.visited = set()
        self.count += 1
        
        s_start_new = None # 第一个碰撞点，即当前规划的起点
        while s != self.s_goal:
            if self.is_collision(s, self.PARENT[s]):
                if s_start_new == None:
                    s_start_new = s
                self.modify(s)
                continue
            s = self.PARENT[s]

        self.s_start_new = s_start_new
        self.path = self.extract_path(self.s_start_new, self.s_goal)
        path = [(item[0]-1, item[1]-1)for item in self.path]
        return path
    
    # 指定新规划的起点和附近的障碍，开始规划
    def get_new_path2(self, s_start_new, obscale_list):
        for item in obscale_list:
            self.obs.add((item[0]+1, item[1]+1))
        
        s_start_new = (s_start_new[0]+1, s_start_new[1]+1)
        s = s_start_new
        self.visited = set()
        self.count += 1
        
        while s != self.s_goal:
            if self.is_collision(s, self.PARENT[s]):
                self.modify(s)
                continue
            s = self.PARENT[s]

        self.s_start_new = s_start_new
        self.path = self.extract_path(self.s_start_new, self.s_goal)
        path = [(item[0]-1, item[1]-1)for item in self.path]
        return path
    
class Map:
    def __init__(self):
        self.node_matrix = []
        self.obstacle_list = []
        self.env = None # 公共的障碍对象
    
    def init_map(self, ch):
        logger.debug("init_map begin ......")
        for x in range(n):
            tmp_node_list = []
            for y in range(n):
                item = ch[x][0][y]
                tmp_node_list.append(Node(x, y, item))
                if item == '#' or item == '*':
                    # 海洋或陆地为障碍部分
                    self.obstacle_list.append((x, y)) # 障碍部分，与当前地图的坐标不同
            self.node_matrix.append(tmp_node_list)
        # 后续D*所需要的环境对象
        self.env = NewEnv(n, n)
        self.env.obs = self.env.set_obs(self.obstacle_list)
        # logger.debug(f"obstacle_list length:{len(self.obstacle_list)}")
        # 0号泊位确定位置
        m.bfs_berth(0, berth[0].x, berth[0].y)
        logger.debug("init_map end ......")

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
    logger.debug("Init begin......")
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
    global boat_capacity # 全局变量的声明
    boat_capacity = int(input())
    m.init_map(ch) # TODO: 初始化地图，保存障碍对象
    # OK确定
    okk = input()
    print("OK")
    logger.debug("Init end......")
    sys.stdout.flush()

# 接受单帧输入
def Input():
    global id, money
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

flag = False # 第0个机器人把第0个货物运达第0个泊位的标识符

# 单帧输出
def Output():
    logger.debug(f"{id}")
    if id==1:
        # 初始：0号轮船到达0号泊位
        print(f"ship 0 0")
        logger.debug(f"transport_time: {berth[0].transport_time}") # 泊位到虚拟点的时间

    # TODO: 所有机器人调度
    for i in range(10):
        robot_i = robot[i]
        if robot_i.aim_type == "" and (not len(gds_pos_statck)==0):
            # 没有带货，规划到最新的可达货物的路径
            logger.info(f"robot({i})规划开始")
            new_good = gds_pos_statck.pop()
            good_node = m.node_matrix[new_good[0]][new_good[1]]
            while not good_node.berth_visited_list[0]:
                logger.debug(f"{new_good} unavailable!")
                new_good = gds_pos_statck.pop()
                good_node = m.node_matrix[new_good[0]][new_good[1]] 
            logger.debug(f"{new_good} available!")
            start = (robot_i.x, robot_i.y)
            # 使用D*初次规划
            # 构建代码时尝试往berth0走
            # new_good = (berth[0].x, berth[0].y) 
            # logger.debug("try to berth(0)")
            robot_i.dstar = NewDStar(start, new_good, m.env)
            path = robot_i.dstar.get_path()
            dirction_dict = {
                (0, 1) : RIGHT,
                (0, -1) : LEFT,
                (1, 0) : DOWN,
                (-1, 0) : UP,
            }
            pos_direction_dict = {}
            for j in range(len(path)-1):
                pre = path[j]
                next = path[j+1]
                pos_direction_dict[pre] = dirction_dict[next[0]-pre[0], next[1]-pre[1]]
            # pos_direction_dict = m.pos_A_star((robot_i.x, robot_i.y), new_good) # A*不再是方向向量了，获得路径用字典存储，方便后续的碰撞后的路径恢复
            logger.info(f"robot({i})规划结束")
            logger.info(f"{pos_direction_dict}")
            robot_i.aim_type = "good"
            robot_i.aim_pos = new_good 
            robot_i.pos_direction_dict = pos_direction_dict
        elif robot_i.aim_type == "good":
            # 移动到货物的位置
            robot_pos = (robot_i.x, robot_i.y)
            logger.debug(f"robot({i}) : ({robot_pos})->{robot_i.aim_pos}")
            # if len(robot_i.aim_pos) > 0:
            if not robot_i.aim_pos == robot_pos:
                # direction = robot_i.direction_list.pop()
                direction = robot_i.pos_direction_dict[robot_pos]
                print(f"move {i} {direction}")
            else:
                print(f"get {i}")
                logger.info(f"robot({i})货物get成功")
                robot_i.aim_type = "wait"
                # robot_i.aim_type = "berth"
        elif robot_i.aim_type == "berth":
            # 带货了，移动到0号泊位
            node = m.node_matrix[robot_i.x][robot_i.y]
            direction = node.berth_best_direction_list[0]
            logger.debug(f"robot({i}) : {(robot_i.x, robot_i.y)}->{(berth[0].x, berth[0].y)}")
            if node.type == '.':
                print(f"move {i} {direction}")
            else:
                # 到了泊位就放下
                print(f"pull {i}")
                logger.info(f"robot({i})货物pull成功")
                robot_i.aim_type = ""
                berth[0].good_queue.put(0) # 随便放一个元素，只是占位置用的
    
    # TODO: 所有轮船调度
    # for i in range(5):
    for i in range(1):
        boat_i = boat[i]
        if (boat_i.status == 1) and (not boat_i.pos == -1):
            # 轮船到达泊位
            logger.debug(f"boat({i}) reach berth({boat_i.pos})")
            berth[boat_i.pos].boat_id = boat_i.id

    # # TODO: 所有泊位调度
    # # for i in range(berth_num):
    # for i in range(1):
    #     berth_i = berth[i]
    #     if not berth_i.boat_id == -1:
    #         # 当前泊位有船则开始装货
    #         for j in range(berth_i.loading_speed):
    #             berth_i.good_queue.get() # 取出一个货
    #             berth_i.loaded_good_num += 1
    #             logger.debug(f"berth({i}) : {berth_i.loaded_good_num }/{boat_capacity}")
    #             # if berth_i.loaded_good_num == boat_capacity:
    #             if berth_i.loaded_good_num == 3: # 测试一下装满3个货就走
    #                 # 货装满了，轮船出发
    #                 print(f"go {berth_i.boat_id}")
    #                 logger.debug(f"berth({i}) full, boat({berth_i.boat_id}) go!")
    #                 berth_i.boat_id = -1
    #                 berth_i.loaded_good_num = 0
    #                 break

    # TODO: 0号轮船等到最后才走压着帧数
    max_frame = 15000
    if id == max_frame - berth[0].transport_time:
        print("go 0") # 有标记了，装上货物后开往虚拟点
        logger.info(f"轮船出发")
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
