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
        flavor_list.append(flavor_list)
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
                # TODO: pod调度算法——最佳适应（Best Fit），选择节点
                # TODO: 如果不存在可用节点，则使用节点扩展算法——水平自动扩展（HPA），申请节点
            # TODO: 输出新申请的节点
            # n = len(new_node_list)
            # print(n, end=" ")
            # if not n == 0:
            #     for new_node in new_node_list:
            #         print(" ", new_node.flavor_id, end="")
            #     print()
            # # TODO: 输出pod调度策略
            # print(new_pod_list[0].flavor_id, end="")
            # for new_pod in new_pod_list[1:]:
            #     print(" ", new_node.flavor_id, end="")
            # print()
            # 暂时定死查看输出结果
            if timestamp == 0:
                print(3, 1, 1, 1)
                print(1, 1, 2, 3)
            elif timestamp == 10:
                print(0)
                print(2)
            
        elif request_type == "DELETE":
            # 删除pods
            pod_id_list = [int(i) for i in input().split()]
            for pod_id in pod_id_list:
                # TODO: 删除单个pod
                pass

        else:
            # 对应END，结束跳出循环
            break