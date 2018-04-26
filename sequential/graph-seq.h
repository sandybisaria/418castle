#ifndef GRAPH_SEQ_H
#define GRAPH_SEQ_H

#include <vector>

class Graph {
public:
    Graph(int num_vertices);
    void addEdges(int u, int v, int w);
    long maxFlow(int s, int t);

private:
    bool findPath(int s, int d, std::vector<int>& parentP);
    int num_vertices;
    std::vector< std::vector< int > > graph;
};

#endif