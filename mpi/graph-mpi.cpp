#include <climits>
#include <iostream>
#include <queue>

#include "graph-mpi.h"

Graph::Graph(int num_vertices){
    graph.assign(num_vertices, std::vector<int>(num_vertices, 0));
    this->num_vertices = num_vertices;
} 

void Graph::addEdges(int u, int v, int w){
    graph[u][v] = w;
}

long Graph::maxFlow(int s, int t){


    return 0;
}



