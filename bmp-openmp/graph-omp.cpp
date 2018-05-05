#include <climits>
#include <iostream>
#include <queue>
#include <tuple>
#include <omp.h>
#include <atomic>

#include "graph-omp.h"

Graph::Graph(int num_vertices){
    graph.assign(num_vertices, std::vector<int>(num_vertices, 0));
    this->num_vertices = num_vertices;
}

void Graph::addEdges(int u, int v, int w){
    graph[u][v] = w;
}

int Graph::getDegree(int u){
    int deg = 0;
    for (int i = 0; i < num_vertices; i++){
        if (graph[u][i] > 0){
            deg+=1;
        }
    }
    return deg;
}




long Graph::parallelKarpSipser(){
    long result;
    std::set<std::tuple<int, int>> M;
    std::set<int> Q;
    std::set<int> Qstar;
    std::vector<int> visited;
    //parallelize this
    visited.assign(num_vertices, 0);
    //parallelize this
    for (int x = 0; x < sizeX; x++){
        if (getDegree(x) == 1){
            Q.insert(x);
        }
        else{
            Qstar.insert(x);
        }
    }
    for(auto u: Q) {
        matchAndUpdate(M,visited,u);
    }
    for(auto u: Qstar) {
        matchAndUpdate(M,visited,u);
    }
    result = M.size();
    return result;

}

void Graph::matchAndUpdate(std::set<std::tuple<int, int>>& M,std::vector<int>& visited, int u){
    //need to figure out fetch and add part

}

