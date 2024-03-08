
import sys
import random
from queue import Queue

# 输出调试日志
import logging
logging.basicConfig(level=logging.INFO,
                    filename="main.log",  # 指定日志文件名
                    filemode="w",  # 指定写入模式，'w'表示覆盖写入，'a'表示追加写入
                    format='%(asctime)s - %(levelname)s - %(message)s')

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
        self.visited_list = [False for i in range(berth_num)] # 是否有到各个泊位的路径
        self.best_direction_list = [RIGHT for i in range(berth_num)] # 到该泊位路径的最短方向

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

    def bfs(self, berth_id, berth_x, berth_y):
        # 队列初始化
        q = Queue()
        # 对上下左右边上四周的16个点初始化，只对陆地点.设置
        if berth_x > 0:
            for y in range(berth_y, berth_y+4):
                node = self.node_matrix[berth_x-1][y]
                if node.type == '.':
                    q.put((node.x, node.y))
                    node.visited_list[berth_id] = True
                    node.best_direction_list[berth_id] = DOWN
        if berth_x < n-1:
            for y in range(berth_y, berth_y+4):
                node = self.node_matrix[berth_x+1][y]
                if node.type == '.':
                    q.put((node.x, node.y))
                    node.visited_list[berth_id] = True
                    node.best_direction_list[berth_id] = UP
        if berth_y > 0:
            for x in range(berth_x, berth_x+4):
                node = self.node_matrix[x][berth_y-1]
                if node.type == '.':
                    q.put((node.x, node.y))
                    node.visited_list[berth_id] = True
                    node.best_direction_list[berth_id] = RIGHT
        if berth_y < n-1:
            for x in range(berth_x, berth_x+4):
                node = self.node_matrix[x][berth_y-1]
                if node.type == '.':
                    q.put((node.x, node.y))
                    node.visited_list[berth_id] = True
                    node.best_direction_list[berth_id] = LEFT
        
        # 深度优先遍历
        while not q.empty():
            x, y = q.get()
            node = self.node_matrix[x][y]
            for (neighbor_x, neighbor_y), direction in self.get_neighbor_list(x, y):
                neighbor_node = self.node_matrix[neighbor_x][neighbor_y]
                if neighbor_node.type == '.' and neighbor_node.visited_list[berth_id]==False:
                    # 对没有访问过的陆地点设置
                    neighbor_node.visited_list[berth_id] = True
                    neighbor_node.best_direction_list[berth_id] = direction
                    q.put((neighbor_x, neighbor_y))

m = Map()

money = 0
boat_capacity = 0
id = 0
ch = []
gds = [[0 for _ in range(N)] for _ in range(N)]

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
    m.bfs(0, berth[0].x, berth[0].y)
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
    # 所有轮船，是否带货、坐标x、坐标y、是否碰撞后恢复
    for i in range(robot_num):
        robot[i].goods, robot[i].x, robot[i].y, robot[i].status = map(int, input().split())
    # 所有轮船，状态、目标泊位
    for i in range(5):
        boat[i].status, boat[i].pos = map(int, input().split())
    okk = input()
    return id

# 单帧输出
def Output(id):
    if id==1:
        print(f"ship 0 0")
        for i in ch:
            logging.info(str(i))
            # 打印所有点到0号泊位的最短路径
        for i in range(n):
            row = ""
            for j in range(n):
                node = m.node_matrix[i][j]
                if node.type == '.':
                    direction = node.best_direction_list[0]
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
            logging.info(row)
    # 0号机器人移动到固定位置0号泊位
    robot_0 = robot[0]
    node = m.node_matrix[robot_0.x][robot_0.y]
    direction = node.best_direction_list[0]
    logging.info(f"({node.x},{node.y}),{direction}")
    print(f"move 0 {node.best_direction_list[0]}")
    return

if __name__ == "__main__":
    Init()
    for zhen in range(1, 15001):
        id = Input()
        Output(id)
        for i in range(robot_num):
            print("move", i, random.randint(0, 3))
            sys.stdout.flush()
        print("OK")
        sys.stdout.flush()
