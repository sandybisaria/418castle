#ifndef GRAPH_SEQ_H
#define GRAPH_SEQ_H

#include <vector>

class Graph {
public:
    Graph(int num_vertices, int b);
    void addEdges(int u, int v, int w);
    int bipartiteMatching();

private:
    int num_vertices;
    std::vector< std::vector< int > > graph;
    
    int a_size, b_size;

    bool findPath(int s, int d, std::vector<int>& parentP);
    long maxFlow(int s, int t);
};

#endif