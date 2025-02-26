# 输出调试日志
import logging
logger = logging.getLogger()
# 文件保存
file_handler = logging.FileHandler("main_get_input.log", "w", encoding="utf-8")
logger.addHandler(file_handler)
# 格式
# formatter = logging.Formatter('%(asctime)s - %(levelname)s - %(message)s')
# file_handler.setFormatter(formatter)
# 日志级别
logger.setLevel(logging.DEBUG)

import sys
import random
n = 200
robot_num = 10
berth_num = 10
N = 210

class Robot:
    def __init__(self, startX=0, startY=0, goods=0, status=0, mbx=0, mby=0):
        self.x = startX
        self.y = startY
        self.goods = goods
        self.status = status
        self.mbx = mbx
        self.mby = mby

robot = [Robot() for _ in range(robot_num + 10)]

class Berth:
    def __init__(self, x=0, y=0, transport_time=0, loading_speed=0):
        self.x = x
        self.y = y
        self.transport_time = transport_time
        self.loading_speed = loading_speed

berth = [Berth() for _ in range(berth_num + 10)]

class Boat:
    def __init__(self, num=0, pos=0, status=0):
        self.num = num
        self.pos = pos
        self.status = status

boat = [Boat() for _ in range(10)]


money = 0
boat_capacity = 0
id = 0
ch = []
gds = [[0 for _ in range(N)] for _ in range(N)]

def Init():
    logger.debug("Init......")
    for i in range(0, n):
        line = input()
        ch.append([c for c in line.split(sep=" ")])
        logger.debug(f"{ch[i]}")
    for i in range(berth_num):
        line = input()
        logger.debug(line)
        berth_list = [int(c) for c in line.split(sep=" ")]
        id = berth_list[0]
        berth[id].x = berth_list[1]
        berth[id].y = berth_list[2]
        berth[id].transport_time = berth_list[3]
        berth[id].loading_speed = berth_list[4]
    boat_capacity = int(input())
    logger.debug(boat_capacity)
    okk = input()
    logger.debug(okk)
    print("OK")
    sys.stdout.flush()

def Input():
    id, money = map(int, input().split(" "))
    logger.debug(f"{id}, {money}")
    num = int(input())
    for i in range(num):
        x, y, val = map(int, input().split())
        logger.debug(f"x={x}, y={y}, val={val}")
        gds[x][y] = val
    for i in range(robot_num):
        robot[i].goods, robot[i].x, robot[i].y, robot[i].status = map(int, input().split())
        logger.debug(f"{robot[i].goods}, {robot[i].x}, {robot[i].y}, {robot[i].status}")
    for i in range(5):
        boat[i].status, boat[i].pos = map(int, input().split())
        logger.debug(f"{boat[i].status}, {boat[i].pos}")
    okk = input()
    logger.debug(okk)
    return id

if __name__ == "__main__":
    Init()
    for zhen in range(1, 15001):
        id = Input()
        for i in range(robot_num):
            print("move", i, random.randint(0, 3))
            sys.stdout.flush()
        print("OK")
        sys.stdout.flush()
