#include <climits>
#include <iostream>
#include <queue>

#include "graph-cuda.h"

Graph::Graph(int num_vertices){
    this->nodes.assign(num_vertices+1,0);
    this->levels.assign(num_vertices,0);
    this->graph.assign(num_vertices, std::vector<int>(num_vertices, 0));
    this->num_vertices = num_vertices;
    this->num_edges = 0;
    this->idx = 0;
}

void Graph::addEdges(int u, int v, int w){
    if (graph[v][u] == 0) { // Backward edge not added yet
        this->num_edges += 2;
        graph[u][v] = w;
        graph[v][u] = -1;
    } else {
        graph[u][v] = w;
    }
}

void Graph::getCudaGraph(){
    this->edges.assign(this->num_edges,0);
    for (int i = 0; i < num_vertices; i++){
        nodes[i] = idx;
        for (int j = 0; j < num_vertices; j++){
            if (graph[i][j] > 0){
                edges[idx] = j;
                idx++;
            } else if (graph[i][j] == -1) {
                // Backward edge, update to 0 capacity
                graph[i][j] = 0;
                edges[idx] = j;
                idx++;
            }
        }
    }
    nodes[num_vertices] = num_edges;

    for (int node = 0; node < nodes.size(); node++) {
        cout << nodes[node] << ' ';
    }
    cout << endl;
    for (int edge = 0; edge < edges.size(); edge++) {
        cout << edges[edge] << ' ';
    }
    cout << endl << endl;
    for (int i = 0; i < num_vertices; i++){
        for (int j = 0; j < num_vertices; j++){
            cout << graph[i][j] << ' ';
        }
        cout << endl;
    }
}