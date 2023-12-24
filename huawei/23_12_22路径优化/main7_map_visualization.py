import matplotlib.pyplot as plt
import networkx as nx


def visualization(map, provider, consumer_vector, transmitter_vector):
    node_list = [] #  其中元素为[[x,y], node_type, label]
    edge_list = [] # 其中元素为[[x1,y1], [x2,y2]]

    def dfs(map, x, y, last, consumer_vector, transmitter_vector):
        node = map.node_matrix[x][y]
        node_list.append([(node.x, node.y), node.node_type, f"{node.node_type[0]}{node.type_id}" if not node.node_type=="EMPTY" else ""]) # 添加结点
        if len(node.child_xy_list)>=2:
            # 分支点必然拐弯
            # 不创建transmitter
            # transmitter = Transmitter(len(transmitter_vector)+1, node.x, node.y)
            # last.target_vector.append([0, transmitter.id, 0])
            # transmitter_vector.append(transmitter)
            transmitter = transmitter_vector[node.type_id-1] # 读取tranmitter
            # 遍历所有孩子
            for i in range(len(node.child_xy_list)):
                child_xy = node.child_xy_list[i]
                child_x, child_y = child_xy[0], child_xy[1]
                edge_list.append([(x, y), (child_x, child_y)]) # 添加边
                dfs(map, child_x, child_y, transmitter, consumer_vector, transmitter_vector)
        elif len(node.child_xy_list)==1:
            child_x, child_y = node.child_xy_list[0][0], node.child_xy_list[0][1]
            edge_list.append([(x, y), (child_x, child_y)]) # 添加边
            if (node.best_direction=="UP" and node.x-child_x==-1 and node.y-child_y==0) \
                or (node.best_direction=="RIGHT" and node.x-child_x==0 and node.y-child_y==1)\
                or (node.best_direction=="DOWN" and node.x-child_x==1 and node.y-child_y==0) \
                or (node.best_direction=="LEFT" and node.x-child_x==0 and node.y-child_y==-1):
                # 没有拐弯
                dfs(map, child_x, child_y, last, consumer_vector, transmitter_vector)
            else:
                # 拐弯了
                # 不创建transmitter
                # transmitter = Transmitter(len(transmitter_vector)+1, node.x, node.y)
                # last.target_vector.append([0, transmitter.id, 0])
                # transmitter_vector.append(transmitter)
                transmitter = transmitter_vector[node.type_id-1] # 读取tranmitter
                dfs(map, child_x, child_y, transmitter, consumer_vector, transmitter_vector)
        else:
            # 叶子节点即遇到了Consumer
            consumer = consumer_vector[node.type_id]
            last.target_vector.append([1, consumer.id, consumer.code_format])


    # 从Provider开始DFS
    def dfs_tree(map, provider, consumer_vector, transmitter_vector):
        
        provider_node = map.node_matrix[provider.x][provider.y]
        node_list.append([(provider.x, provider.y), provider_node.node_type, "P"]) # 添加结点
        for i in range(len(provider_node.child_xy_list)):
            child_xy = provider_node.child_xy_list[i]
            child_x, child_y = child_xy[0], child_xy[1]
            edge_list.append([(provider.x, provider.y), (child_x, child_y)]) # 添加边
            dfs(map, child_x, child_y, provider, consumer_vector, transmitter_vector)
        return node_list , edge_list

    # 手动反转x，y坐标，纵向为x横向为y
    def inverse_xy(node_list, edge_list):
        inversed_node_list = []
        inversed_edge_list = []
        for node in node_list:
            node[0] = tuple(reversed(node[0]))
            inversed_node_list.append(node)
        for edge in edge_list:
            edge[0] = tuple(reversed(edge[0]))
            edge[1] = tuple(reversed(edge[1]))
            inversed_edge_list.append(edge)
        return inversed_node_list, inversed_edge_list


    figure, ax = plt.subplots(figsize=(8,8))

    # 对象创建
    G = nx.DiGraph()
    pos = {}# 结点位置
    # 手动反转x，y坐标，纵向为x横向为y
    node_list = [] #  其中元素为[[x,y], node_type, label]
    edge_list = [] # 其中元素为[[x1,y1], [x2,y2]]
    node_list, edge_list = dfs_tree(map, provider, consumer_vector, transmitter_vector)
    node_list, edge_list = inverse_xy(node_list, edge_list)
    for node in node_list:
        G.add_node(node[0], node_type=node[1], label=node[2]) # 结点用其坐标标识
        pos[node[0]] = node[0]
    for edge in edge_list:
        G.add_edge(edge[0], edge[1])

    # 结点绘制
    color_dict = {
        "PROVIDER" : "#FF000080",
        "CONSUMER" : "#00FF0080",
        "TRANSMITTER" : "#0000FF80",
        "EMPTY": "white" # 空地设置为白色背景色
    }
    node_color_list = [color_dict[i] for i in nx.get_node_attributes(G, "node_type").values()]
    node_label_dict =  nx.get_node_attributes(G, "label")
    nx.draw(G, pos, node_color=node_color_list, labels=node_label_dict) # 绘制结点


    # 网格线设置
    # ax.grid(color="grey", linestyle="--") # 整数位置画网格
    N = map.N
    positions = [i-0.5 for i in range(N)]
    for pos in positions:
        ax.axhline(y=pos, color="grey", linestyle="--")
        ax.axvline(x=pos, color="grey", linestyle="--")

    # 网格中单元格代价绘制
    for i in range(N):
        for j in range(N):
            node = map.node_matrix[i][j]
            text = str(node.weight)
            x = node.x-0.3
            y = node.y-0.3
            ax.text(y, x, text) # 稍微偏移

    # 刻度设置
    ax.axis("on")
    ax.set_xlim(left=-0.5, right=N-0.5) # 设置坐标轴范围，自动设置为整数
    ax.set_ylim(bottom=N-0.5, top=-0.5)
    ax.tick_params(left=True, bottom=True, labelleft=True, labelbottom=True)
    ax.xaxis.tick_top() # 坐标原点移动到左上角
    plt.show()