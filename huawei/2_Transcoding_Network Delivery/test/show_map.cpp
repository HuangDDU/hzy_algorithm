N = node_map.N

for(int i=0; i<node_map.N; i++){
    for(int j=0; j<node_map.N; j++){
        Node& node = node_map.node_matrix[i][j]
        cout << node.distance << " ";
    }
    cout << endl;
}
cout << "==================================" << endl;

for(int i=0; i<N; i++){
    for(int j=0; j<N; j++){
        Node& node = node_map.node_matrix[i][j]
        cout << node.distance << " ";
        if (node.best_direction == "UP"){
            cout << "↑" << " ";
        }elif (node.best_direction == "UP"){
            cout << "→" << " ";
        }elif (node.best_direction == "UP"){
            cout << "↓" << " ";
        }elif (node.best_direction == "UP"){
            cout << "←" << " ";
        }else{
            cout << "U" << " ";
        }
    }
    cout << endl;
}