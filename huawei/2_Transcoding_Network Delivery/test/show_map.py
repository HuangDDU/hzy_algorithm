N = map.N

for i in range(N):
    for j in range(N):
        node = map.node_matrix[i][j]
        print(node.distance, end=" ")
    print()
print("==================================")
for i in range(N):
    for j in range(N):
        node = map.node_matrix[i][j]
        if node.best_direction == "UP":
            print("↑", end=" ")
        elif node.best_direction == "RIGHT":
            print("→", end=" ")            
        elif node.best_direction == "DOWN":
            print("↓", end=" ")            
        elif node.best_direction == "LEFT":
            print("←", end=" ")
        else:
            print("U", end=" ")
    print()