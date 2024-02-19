import sys
from util import log_node_list

class Flavor:
    def __init__(self, flavor_id, max_cpu, max_memory, price_per_second) -> None:
        self.flavor_id = flavor_id
        self.max_cpu = max_cpu
        self.max_memory = max_memory
        self.price_per_second = price_per_second
        
class Pod:
    def __init__(self, pod_id, cpu, memory) -> None:
        self.pod_id = pod_id
        self.cpu = cpu
        self.memory = memory
        self.placed_node_id = -1
        self.is_available = False

class Node:
    def __init__(self, node_id, flavor_id, max_cpu, max_memory) -> None:
        self.node_id = node_id
        self.flavor_id = flavor_id
        self.max_cpu = max_cpu
        self.max_memory = max_memory
        self.now_cpu = 0
        self.now_memory = 0
        self.left_cpu = max_cpu
        self.left_memory = max_memory
        self.is_available = True # 是否可用的
    

if __name__=="__main__":
    # 节点类型
    F = int(input())
    flavor_list = []
    for i in range(F):
        line_list = input().split()
        flavor_id = i
        max_cpu = int(line_list[0])
        max_memory = int(line_list[1])
        price_per_second = float(line_list[2])
        flavor = Flavor(flavor_id, max_cpu, max_memory, price_per_second)
        flavor_list.append(flavor)
    node_list = []
    pod_list = []
    # 循环处理请求
    while True:
        line_list = input().split()
        timestamp = int(line_list[0])
        request_type = line_list[1]
        S = int(line_list[2])
        if request_type == "CREATE":
            # 创建pods
            new_pod_list = []
            new_node_list = []
            for i in range(S):
                line_list = input().split()
                pod_id = int(line_list[0])
                cpu = int(line_list[1])
                memory = int(line_list[2])
                # TODO: 创建单个pod
                pod = Pod(pod_id, cpu, memory)
                new_pod_list.append(pod)
                pod_list.append(pod)
                # TODO: pod调度算法——最佳适应（Best Fit）(寻找能满足要求的最小空间)，选择节点
                best_node = None
                best_fit_value = float("inf") # 适应值，该节点分配后剩余的空间，越小越好
                for node in node_list:
                    if node.left_cpu >= pod.cpu and node.left_memory >= pod.memory and node.is_available:
                        fit_value = max(node.left_cpu-pod.cpu, node.left_memory-pod.memory)
                        if fit_value < best_fit_value:
                            # 寻找到更优的节点
                            best_fit_value = fit_value
                            best_node = node
                if not best_node == None:
                    pod.placed_node_id = best_node.node_id
                    # 减少资源
                    best_node.now_cpu += pod.cpu
                    best_node.now_memory += pod.memory
                    best_node.left_cpu -= pod.cpu
                    best_node.left_memory -= pod.memory
                else:
                    # TODO: 如果不存在可用节点，则使用节点扩展算法，申请节点
                    best_flavor = None
                    best_price_per_second = float("inf")
                    for flavor in flavor_list:
                        if flavor.max_cpu >= pod.cpu and flavor.max_memory >= pod.memory:
                            # 寻找到更优的节点类型（暂时找到就跳出）
                            if flavor.price_per_second < best_price_per_second:
                                best_price_per_second = flavor.price_per_second
                                best_flavor = flavor
                    node = Node(len(node_list), best_flavor.flavor_id, best_flavor.max_cpu, best_flavor.max_memory)
                    pod.placed_node_id = node.node_id
                    node.now_cpu += pod.cpu
                    node.now_memory += pod.memory
                    node.left_cpu -= pod.cpu
                    node.left_memory -= pod.memory
                    new_node_list.append(node)
                    node_list.append(node)
            # TODO: 输出新申请的节点
            n = len(new_node_list)
            print(n, end="")
            if not n == 0:
                print(" ", end="")
                for new_node in new_node_list:
                    print(" ", new_node.flavor_id+1, end="")
            print()
            # TODO: 输出pod调度策略
            print(new_pod_list[0].placed_node_id+1, end="")
            for new_pod in new_pod_list[1:]:
                print(" ", new_pod.placed_node_id+1, end="")
            print()
        elif request_type == "DELETE":
            # 删除pods
            pod_id_list = [int(i) for i in input().split()]
            for pod_id in pod_id_list:
                # TODO: 删除单个pod
                pod = pod_list[pod_id-1]
                pod.is_available = False
                # 释放节点空间
                node = node_list[pod.placed_node_id]
                node.now_cpu -= pod.cpu
                node.now_memory -= pod.memory
                node.left_cpu += pod.cpu
                node.left_memory += pod.memory
                if node.now_cpu == 0 and node.now_memory== 0:
                    node.is_available = False
        else:
            # 对应END，结束跳出循环
            break
        # log_node_list(timestamp, node_list, pod_list, log_file="best_fit_demo.log") # 日志输出，用作可视化
        # log_node_list(timestamp, node_list, pod_list, log_file="best_fit_00_head20.log") # 日志输出，用作可视化
        log_node_list(timestamp, node_list, pod_list, log_file="best_fit_03_head20.log") # 日志输出，用作可视化
    sys.stdout.flush()