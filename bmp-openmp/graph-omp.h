#ifndef GRAPH_SEQ_H
#define GRAPH_SEQ_H

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
    std::vector< int > degree;
  
    int a_size, b_size;

    AdjMat karpSipser();
    void matchAndUpdate(AdjMat& matching, int a, std::vector<bool>& visited);
};

#endif