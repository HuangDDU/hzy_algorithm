
import sys
import random
import numpy as np
from queue import Queue, PriorityQueue
from cbs.cbs import CBS, Environment, Constraints
from cbs.a_star import AStar


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
boat_num = 5
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

        self.berth_id = -1 # 归属的泊位
        self.berth_pos = -1 # 归属的泊位


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
        self.good_queue = Queue() #  货物队列
        self.loaded_good_num = 0 #  已经往当前船上装载的货物数量
        self.robot_id_list = []
        self.robot_pos_dict = {} # 泊位及其停靠位置

berth = [Berth(i) for i in range(berth_num)]


# 轮船
class Boat:
    def __init__(self, id, num=0, pos=0, status=0):
        self.id = id
        self.num = num
        self.pos = pos
        self.status = status


boat = [Boat(i) for i in range(boat_num)]

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
        

# 地图坐标格式的转换
def transfer_pos_to_cbs(pos):
    return pos[1], n-1-pos[0]

def transfer_pos_to_now(pos):
    return pos[1], n-1-pos[0]

agents = []

def get_agents():
    gents = []
    for i in range(len(robot)):
        robot_i = robot[i]
        agents.append({
            "start" : [robot_i.x, robot_i.y],
            # 终点后续确定
            "name" : i
        })
    return agents

class Map:
    def __init__(self):
        self.n = n
        self.node_matrix = []
        self.obstacles = []
        self.agents = []
    
    def init_map(self, ch):
        for x in range(n):
            tmp_node_list = []
            for y in range(n):
                item = ch[x][0][y]
                tmp_node_list.append(Node(x, y, item))
                if item == '#' or item == '*':
                    # 海洋或陆地为障碍部分
                    # self.obstacles.append(y, n-1-x) ，与当前地图的坐标不同
                    self.obstacles.append((x, y)) # 障碍部分，与当前地图的坐标不同
            self.node_matrix.append(tmp_node_list)

        
m = Map()


# CBS的新环境类
class NewEnvironment(Environment):
    def __init__(self, dimension, agents, obstacles):
        super().__init__(dimension, agents, obstacles)
    
    # 重新设置agents
    def reset_agents(self, agents):
        self.agents = agents
        self.agent_dict = {}

        self.make_agent_dict()

        self.constraints = Constraints()
        self.constraint_dict = {}

        self.a_star = AStar(self)


money = 0
boat_capacity = 0
id = 0
ch = []
gds = [[0 for _ in range(N)] for _ in range(N)] # 货物列表
gd0_pos = [-1, -1] # 第一个出现的货物坐标，保存为列表，函数才能更改其内部元素
gds_pos_statck = [] # TODO:使用列表实现的栈，以后可能需要用优先级队列实现
env = None
cbs_obj = None
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
    global boat_capacity # 全局变量的声明
    boat_capacity = int(input())
    # 初始化结点、障碍列表
    m.init_map(ch)
    # TODO: 划分陆地点与泊位的归属关系，不要跑的太远了
    # TODO: 划分机器人与泊位的归属关系
    berth2robot = {
        0: [0, 1],
        1: [2, 3],
        2: [4, 5],
        3: [6, 7],
        4: [8, 9],
    } # berth包含的robot
    robot2berth = {} # robot对应的berth
    # TODO:泊位可装货坐标点确定
    for berth_id, robot_id_list in berth2robot.items():
        berth_i = berth[berth_id]
        robot_pos_dict = {}
        berth_i.robot_id_list = robot_id_list
        for i in range(len(robot_id_list)):
            robot_id = robot_id_list[i]
            robot_i = robot[robot_id]
            robot2berth[robot_id] = berth_id
            robot_i.berth_id = berth_id
            x, y = berth_i.x + int(i/4), berth_i.y + i%4 # 计算泊位上合适的停靠位置
            robot_i.berth_pos = [x,y]
            robot_pos_dict[robot_id] = [x,y]
            logger.debug(f"rebot({robot_id})-berth({berth_id})-load pos{robot_i.berth_pos}")
        berth_i.robot_pos_dict = robot_pos_dict
    # CBS对象创建
    global env, cbs_obj
    env = NewEnvironment(m.n, {}, m.obstacles) #  初始agents为空，只初始化地图元素
    cbs_obj = CBS(env)
    # OK确定
    okk = input()
    logger.debug(okk)
    print("OK")
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
        logger.debug(f"x={x}, y={y}, val={val}")
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
    logger.debug(f"frame:({id})")
    if id==1:
        # 初始
        # 0~5号轮船开始驶向0~5号泊位
        for i in range(boat_num):
            print(f"ship {i} {i}")
            logger.debug(f"ship({i}) plan from vitual to berth({i})")
        agents = get_agents() # 暂时只能获得起点
        # 机器人先集体跑到泊位附近
        for i in range(robot_num):
            robot_i = robot[i]
            agents[i]["goal"] = robot_i.berth_pos
            logger.debug(f"robot({i}) plan to berth({i})")
        # 统一规划机器人路径MAPF
        env.reset_agents(agents)
        logger.debug(f"dimension : {env.dimension}")
        logger.debug(f"agents : {env.agents}")
        logger.debug(f"obstacles : {env.obstacles}")
        path = cbs_obj.search()
        logger.debug(path)
        

    # # TODO: 所有机器人调度
    # for i in range(robot_num):
    #     robot_i = robot[i]
    #     if robot_i.aim_type == "" and (not len(gds_pos_statck)==0):
    #         # 没有带货，规划到最新的可达货物的路径
    #         # new_good = gds_pos_statck.pop()
    #         # good_node = m.node_matrix[new_good[0]][new_good[1]]
    #         # while  not good_node.berth_visited_list[0]:
    #         #     # 该货物不可用
    #         #     logger.debug(f"{new_good} unavailable!")
    #         #     new_good = gds_pos_statck.pop()
    #         #     good_node = m.node_matrix[new_good[0]][new_good[1]] 
    #         # logger.debug(f"{new_good} available!")


    #         logger.info(f"robot({i})规划开始")
    #         new_good = gds_pos_statck.pop()
    #         good_node = m.node_matrix[new_good[0]][new_good[1]]
    #         while  not good_node.berth_visited_list[0]:
    #             logger.debug(f"{new_good} unavailable!")
    #             new_good = gds_pos_statck.pop()
    #             good_node = m.node_matrix[new_good[0]][new_good[1]] 
    #         logger.debug(f"{new_good} available!")
    #         pos_direction_dict = m.pos_A_star((robot_i.x, robot_i.y), new_good) # A*不再是方向向量了，获得路径用字典存储，方便后续的碰撞后的路径恢复
    #         logger.info(f"robot({i})规划结束")
    #         robot_i.aim_type = "good"
    #         # robot_i.direction_list = direction_list
    #         robot_i.aim_pos = new_good 
    #         robot_i.pos_direction_dict = pos_direction_dict
    #     elif robot_i.aim_type == "good":
    #         # 移动到货物的位置
    #         robot_pos = (robot_i.x, robot_i.y)
    #         logger.debug(f"robot({i}) : ({robot_pos})->{robot_i.aim_pos}")
    #         # if len(robot_i.aim_pos) > 0:
    #         if not robot_i.aim_pos == robot_pos:
    #             # direction = robot_i.direction_list.pop()
    #             direction = robot_i.pos_direction_dict[robot_pos]
    #             print(f"move {i} {direction}")
    #         else:
    #             print(f"get {i}")
    #             logger.info(f"robot({i})货物get成功")
    #             robot_i.aim_type = "berth"
    #     elif robot_i.aim_type == "berth":
    #         # 带货了，移动到0号泊位
    #         node = m.node_matrix[robot_i.x][robot_i.y]
    #         direction = node.berth_best_direction_list[0]
    #         logger.debug(f"robot({i}) : {(robot_i.x, robot_i.y)}->{(berth[0].x, berth[0].y)}")
    #         if node.type == '.':
    #             print(f"move {i} {direction}")
    #         else:
    #             # 到了泊位就放下
    #             print(f"pull {i}")
    #             logger.info(f"robot({i})货物pull成功")
    #             robot_i.aim_type = ""
    #             berth[0].good_queue.put(0) # 随便放一个元素，只是占位置用的
    
    # # TODO: 所有轮船调度
    # # for i in range(5):
    # for i in range(1):
    #     boat_i = boat[i]
    #     if (boat_i.status == 1) and (not boat_i.pos == -1):
    #         # 轮船到达泊位
    #         logger.debug(f"boat({i}) reach berth({boat_i.pos})")
    #         berth[boat_i.pos].boat_id = boat_i.id

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

    # # TODO: 0号轮船等到最后才走压着帧数
    # max_frame = 15000
    # if id == max_frame - berth[0].transport_time:
    #     print("go 0") # 有标记了，装上货物后开往虚拟点
    #     logger.info(f"轮船出发")
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
