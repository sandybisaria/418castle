#include <climits>
#include <iostream>
#include <queue>

#include "graph-seq.h"

#define SOURCE (num_vertices) // graph[num_vertices] is source
#define SINK (num_vertices+1) // graph[num_vertices+1] is sink
#define SD(i) ((i) - a_size)
#define SU(i) ((i) + a_size)

Graph::Graph(int num_vertices, int b){
    graph.assign(num_vertices+2, std::vector<int>(num_vertices + 2, 0));
    this->num_vertices = num_vertices;

    a_size = b;
    b_size = num_vertices - b;
} 

void Graph::addEdges(int u, int v, int w) {
    graph[u][v] = w;
}

int Graph::bipartiteMatching() {
    for (int i = 0; i < a_size; i++) {
        graph[SOURCE][i] = 1;
    }
    for (int i = 0; i < b_size; i++) {
        graph[SU(i)][SINK] = 1;
    }

    // Solve maximum flow on this graph
    return maxFlow(SOURCE, SINK);
}

long Graph::maxFlow(int s, int t){
    long max_flow = 0;
    std::vector<int> parentP(num_vertices+2, 0);
    while (findPath(s, t, parentP)){
        long flow = LONG_MAX;
        for (int v = t; v != s; v = parentP[v]){
            int u = parentP[v];
            flow = std::min(flow, (long) graph[u][v]);
        }
        for (int v = t; v != s; v = parentP[v]){
            int u = parentP[v];
            graph[u][v] -= flow;
            graph[v][u] += flow;
        }
        max_flow += flow;
    }
    return max_flow;
}

bool Graph::findPath(int s, int d, std::vector<int>& parentP){
    static std::vector<bool> visited(num_vertices+2, false);
    std::queue<int> q;
    q.push(s);
    visited[s] = true;
    parentP[s] = -1;
    while (!q.empty()){
        int u = q.front();
        q.pop();
        for (int v = 0; v < num_vertices+2; v++){
            if (visited[v] == false && graph[u][v] > 0){
                q.push(v);
                parentP[v] = u;
                visited[v] = true;
            }
        }
    }

    bool found = (visited[d] == true);
    for (int v = 0; v < num_vertices+2; v++){
        visited[v] = false;
    }
    return found;
}