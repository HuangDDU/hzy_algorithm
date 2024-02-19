import queue

# 创建一个优先队列
q = queue.PriorityQueue()

# 添加元素到队列
q.put((3, 'Apple'))
q.put((1, 'Banana'))
q.put((2, 'Orange'))

# 从队列中获取元素
while not q.empty():
    item = q.get()
    print(item[1])  # 输出元素的值

# 输出结果：
# Banana
# Orange
# Apple