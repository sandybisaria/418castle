#include <climits>
#include <iostream>
#include <queue>

#include "graph-seq.h"

Graph::Graph(int num_vertices){
    graph.assign(num_vertices, std::vector<int>(num_vertices, 0));
    this->num_vertices = num_vertices;
} 

void Graph::addEdges(int u, int v, int w){
    graph[u][v] = w;
}

long Graph::maxFlow(int s, int t){
    long max_flow = 0;
    std::vector<int> parentP(num_vertices, 0);
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
    static std::vector<bool> visited(num_vertices, false);
    std::queue<int> q;
    q.push(s);
    visited[s] = true;
    parentP[s] = -1;
    while (!q.empty()){
        int u = q.front();
        q.pop();
        for (int v = 0; v < num_vertices; v++){
            if (visited[v] == false && graph[u][v] > 0){
                q.push(v);
                parentP[v] = u;
                visited[v] = true;
            }
        }
    }

    bool found = (visited[d] == true);
    for (int v = 0; v < num_vertices; v++){
        visited[v] = false;
    }
    return found;
}

