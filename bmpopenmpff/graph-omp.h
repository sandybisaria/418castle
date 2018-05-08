#ifndef GRAPH_OMP_H
#define GRAPH_OMP_H

#include <vector>
#include <set>

typedef std::vector< std::vector< int > > AdjMat;

class Graph {
public:
    Graph(int num_vertices, int b);
    void addEdges(int u, int v, int w);

    int bipartiteMatching();
private:
    int num_vertices;
    AdjMat graph;
  
    int a_size, b_size;

    long maxFlow(int s, int t);
};

#endif