// #include "policy.h"
#include "topology.h"
#include "sigmoidlayer.h"

#include <iostream>
#include <vector>
#include <stack>

using namespace std;

// 图类定义
class Graph {
    int V; // 顶点数
    vector<vector<int>> adj; // 邻接表

    // DFS 辅助函数
    void DFSUtil(int v, vector<bool>& visited, stack<int>& Stack);

public:
    Graph(int V); // 构造函数
    void addEdge(int v, int w); // 添加边
    void topologicalSort(); // 拓扑排序
};

Graph::Graph(int V) {
    this->V = V;
    adj.resize(V);
}

void Graph::addEdge(int v, int w) {
    adj[v].push_back(w);
}

void Graph::DFSUtil(int v, vector<bool>& visited, stack<int>& Stack) {
    // 标记当前顶点为已访问
    visited[v] = true;

    // 递归访问所有邻接顶点
    for (int i : adj[v]) {
        if (!visited[i])
            DFSUtil(i, visited, Stack);
    }

    // 将当前顶点压入栈中
    Stack.push(v);
    std::cout<<v<<std::endl;
}

void Graph::topologicalSort() {
    stack<int> Stack;
    vector<bool> visited(V, false);

    // 对每个未访问的顶点调用 DFSUtil
    for (int i = 0; i < V; i++) {
        if (!visited[i])
            DFSUtil(i, visited, Stack);
    }

    // 输出栈中的顶点
    while (!Stack.empty()) {
        cout << Stack.top() << " ";
        Stack.pop();
    }
}
    

int main() {
    struct sigmoidSublayer_1;
    using sublayer_1 = Sublayer<sigmoidSublayer_1, SigmoidLayer>;

    struct sigmoidSublayer_2;
    using sublayer_2 = Sublayer<sigmoidSublayer_2, SigmoidLayer>;

    struct LayerInput; struct LayerOutput;
    struct sigmoid1_input;
    struct sigmoid1_output;
    struct sigmoid2_input;
    struct sigmoid2_output;
    struct sigmoid3_input;
    struct sigmoid3_output;
    struct sigmoidSublayer_3;
    using sublayer_3 = Sublayer<sigmoidSublayer_3, SigmoidLayer>;
    using inconnect = InConnect<LayerInput, sublayer_2, sigmoid2_input>;
    using inntrconnect = InternalConnect<sigmoidSublayer_2, sigmoid2_output, sigmoidSublayer_1, sigmoid1_input>;
    using interconnect_1 = InternalConnect<sigmoidSublayer_1, sigmoid1_output, sigmoidSublayer_3, sigmoid3_input>;
    using outconnect = OutConnect<sigmoidSublayer_1, sigmoid1_output, sigmoid1_output>;
    using topo = Topology<sublayer_1, sublayer_2, sublayer_3,inntrconnect, interconnect_1, inconnect, outconnect>;
    // print_sublayers<topo>();
    // print_interconnectlayers<topo>();


    // using res = TopologicalOrdering_<topo::Sublayers, topo::InterConnects>::type;
    // print_after_process<res>(); 
    // print_after_process<InternalLayerSet<topo::InterConnects>>();
    // print_after_process<InternalInLayerSet<topo::InterConnects>>();

    using subres = TopologicalOrdering_<topo::Sublayers, topo::InterConnects>::SublayerPreRes;
     subres::printOrderedTypes();
     subres::printUnorderedTypes();


    //     // 创建一个有向无环图
    // Graph g(6);
    // g.addEdge(5, 2);
    // g.addEdge(0, 5);
    // g.addEdge(0, 4);
    // g.addEdge(1, 4);
    // g.addEdge(3, 2);
    // g.addEdge(1, 3);

    // cout << "拓扑排序结果: ";
    // g.topologicalSort();

    return 0;
}