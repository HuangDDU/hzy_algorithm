#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
using namespace std;

class Flavor
{
public:
    int flavor_id;
    int max_cpu, max_memory;
    float price_per_second;
    Flavor(int flavor_id, int max_cpu,  int max_memory, float price_per_second){
        this->flavor_id = flavor_id;
        this->max_cpu = max_cpu;
        this->max_memory = max_memory;
        this->price_per_second = price_per_second;
    }
};


class Pod
{
public:
    int pod_id;
    int cpu, memory;
    int placed_node_id;
    bool is_available;
    Pod(int pod_id, int cpu, int memory){
        this->pod_id = pod_id;
        this->cpu = cpu;
        this->memory = memory;
        this->placed_node_id = -1;
        this->is_available = true;
    }
};


class  Node
{
public:
    int node_id;
    int flavor_id;
    int max_cpu, max_memory;
    int now_cpu, now_memory;
    int left_cpu, left_memory;
    bool is_available;
    Node(int node_id, int flavor_id, int max_cpu, int max_memory){
        this->node_id = node_id;
        this->flavor_id = flavor_id;
        this->max_cpu = max_cpu;
        this->max_memory = max_memory;
        this->now_cpu = 0;
        this->now_memory = 0;
        this->left_cpu = max_cpu;
        this->left_memory = max_memory;
        this->is_available = true; // 是否可用的
     }
};


int main(){
    // 节点类型
    int F;
    cin >> F;
    vector<Flavor> flavor_list;
    for(int i=0; i<F; i++){
        int flavor_id = i;
        int max_cpu;
        int max_memory;
        float price_per_second;
        cin >> max_cpu >> max_memory >> price_per_second;
        Flavor flavor = Flavor(flavor_id, max_cpu, max_memory, price_per_second);
        flavor_list.push_back(flavor);
    }
    vector<Node> node_list;
    vector<Pod> pod_list;
    while (true)
    {
        int timestamp, S;
        string request_type;
        cin >> timestamp >> request_type >> S;
        if(request_type=="CREATE"){
            vector<Node> new_node_list;
            vector<Pod> new_pod_list;
            for(int i=0; i<S; i++){
                int pod_id, cpu, memory;
                cin >> pod_id >> cpu >> memory;
                // TODO: 创建单个pod
                Pod pod = Pod(pod_id, cpu, memory);
                // pod对象后续还要修改，修改完后再添加上去
                // new_pod_list.push_back(pod);
                // pod_list.push_back(pod);
                // TODO: pod调度算法——最佳适应（Best Fit）(寻找能满足要求的最小空间)，选择节点
                // Node best_node;  // NOTE: 不好指定为空，使用下标查找
                int best_node_index = -1;
                double best_fit_value = numeric_limits<double>::max();
                for(int j=0; j<node_list.size(); j++){
                    Node node = node_list[j]; // 这样做会深拷贝
                    if((node.left_cpu >= pod.cpu)&&(node.left_memory >= pod.memory)&&(node.is_available)){
                        double fit_value = max(node.left_cpu-pod.cpu, node.left_memory-pod.memory);
                        if(fit_value < best_fit_value){
                            // 寻找到更优的节点
                            best_fit_value = fit_value;
                            // best_node = node;
                            best_node_index = j;
                        }
                    }
                }
                if(!(best_node_index == -1)){
                //    // 这里的指针取对象是深拷贝，无法修改属性
                //    Node best_node = *best_node_p;
                //    pod.placed_node_id = best_node.node_id;
                //    // 减少资源
                //    best_node.now_cpu += pod.cpu;
                //    best_node.now_memory += pod.memory;
                //    best_node.left_cpu -= pod.cpu;
                //    best_node.left_memory -= pod.memory;
                    pod.placed_node_id = node_list[best_node_index].node_id;
                    // 减少资源
                    node_list[best_node_index].now_cpu += pod.cpu;
                    node_list[best_node_index].now_memory += pod.memory;
                    node_list[best_node_index].left_cpu -= pod.cpu;
                    node_list[best_node_index].left_memory -= pod.memory;
                }else{
                    // TODO: 如果不存在可用节点，则使用节点扩展算法，申请节点
                    // Flavor best_flavor;
                    int best_flavor_index;
                    float best_price_per_second = numeric_limits<float>::max();
                    for(int j=0; j<flavor_list.size(); j++){
                        Flavor flavor = flavor_list[j];// 这样做会深拷贝
                        if((flavor.max_cpu >= pod.cpu)&&(flavor.max_memory >= pod.memory)){
                            if(flavor.price_per_second < best_price_per_second){
                                best_price_per_second = flavor.price_per_second;
                                best_flavor_index = j;
                            }
                        }
                    }
                    Flavor best_flavor = flavor_list[best_flavor_index];
                    Node node = Node(node_list.size(), best_flavor.flavor_id, best_flavor.max_cpu, best_flavor.max_memory);
                    pod.placed_node_id = node.node_id;
                    node.now_cpu += pod.cpu;
                    node.now_memory += pod.memory;
                    node.left_cpu -= pod.cpu;
                    node.left_memory -= pod.memory;
                    new_node_list.push_back(node);
                    node_list.push_back(node);
                }
                new_pod_list.push_back(pod);
                pod_list.push_back(pod);
            }
            // TODO: 输出新申请的节点
            int n = new_node_list.size();
            cout << n;
            if(!(n==0)){
                for(Node new_node : new_node_list){
                    cout << " " << new_node.flavor_id+1;
                }
            }
            cout << endl;
            // TODO: 输出pod调度策略
            for(int i=0; i<new_pod_list.size(); i++){
                Pod new_pod = new_pod_list[i];
                cout << new_pod.placed_node_id+1;
                if (i < new_pod_list.size()-1){
                    cout << " ";
                }
            }
            cout << endl;
        }else if(request_type=="DELETE"){
            int pod_id;
            for(int i=0; i < S; i++){
                cin >> pod_id;
                // // TODO: 删除单个pod
                // Pod pod = pod_list[pod_id-1];
                // pod_list[pod_id-1].is_available = false;
                // // 释放节点空间
                // Node node = node_list[pod.placed_node_id];
                // node.now_cpu -= pod.cpu;
                // node.now_memory -= pod.memory;
                // node.left_cpu += pod.cpu;
                // node.left_memory += pod.memory;
                // if ((node.now_cpu == 0)&&(node.now_memory == 0)){
                //     node.is_available = false;
                // }
                // TODO: 删除单个pod
                Pod pod = pod_list[pod_id-1];
                pod_list[pod_id-1].is_available = false;
                // 释放节点空间
                int node_id = pod.placed_node_id;
                node_list[node_id].now_cpu -= pod.cpu;
                node_list[node_id].now_memory -= pod.memory;
                node_list[node_id].left_cpu += pod.cpu;
                node_list[node_id].left_memory += pod.memory;
                Node node = node_list[node_id];
                if ((node.now_cpu == 0)&&(node.now_memory == 0)){
                    node_list[node_id].is_available = false;
                }
            }

        }else{
            break;
        }
    }
    cout.flush();
}
    