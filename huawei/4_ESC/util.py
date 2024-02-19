import logging
import json
import os

def log_node_list(timestamp, node_list, pod_list, pod_detail=False, log_file="log.log"):
    logging.basicConfig(level=logging.INFO,
                    filename=log_file,  # 指定日志文件名
                    filemode="w",  # 指定写入模式，'w'表示覆盖写入，'a'表示追加写入
                    format='%(asctime)s - %(levelname)s - %(message)s')
    logging.info(f"=========={timestamp}==========")
    for node in node_list:
        if node.is_available:
            logging.info(f"{node.node_id} | "+
                         f"cpu({(node.now_cpu / node.max_cpu)*100:.2f}%): {node.now_cpu}/{node.max_cpu} | "+
                         f"memory({(node.now_memory / node.max_memory)*100:.2f}%): {node.now_memory}/{node.max_memory}")
            if pod_detail:
                logging.info(node.placed_pod_id_list)
    # logging.info(f"==============================")

def json_node_list(timestamp, node_list, pod_list, json_file="log.json"):
    if os.path.exists(json_file):
        # 读取之前的json文件
        with open(json_file, "r") as f:
            data = json.load(f)
    else:
        # 首次创建json文件
        data = {}
    tmp_data = {}
    for node in node_list:
        if node.is_available:
            tmp_data[node.node_id] = {}
            tmp_data[node.node_id]["global"] = {
                "now_cpu" : node.now_cpu,
                "max_cpu" : node.max_cpu,
                "cpu_rate" : node.now_cpu / node.max_cpu,
                "now_memory" :  node.now_memory,
                "max_memory" :  node.max_memory,
                "memory_rate" : node.now_memory / node.max_memory
            }
            
    data[timestamp] = tmp_data
    with open(json_file, "w") as f:
        json.dump(data, f)