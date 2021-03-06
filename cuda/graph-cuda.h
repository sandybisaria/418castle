#ifndef GRAPH_CUDA_H
#define GRAPH_CUDA_H

#include <vector>

class Graph {
public:
    Graph(int num_vertices);
    void addEdges(int u, int v, int w);
    void getCudaGraph();
    long maxFlow(int s, int t);

private:
    int idx;
    int num_vertices;
    int num_edges;

    std::vector<int> nodes;
    std::vector<int> edges;

    std::vector< std::vector< int > > graph;
};

#endif