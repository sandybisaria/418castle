#ifndef GRAPH_SEQ_H
#define GRAPH_SEQ_H

#include <vector>

class Graph {
public:
    Graph(int num_vertices, int b);
    void addEdges(int u, int v, int w);
    int bipartiteMatching();

private:
    bool bfs();
    bool dfs(int u);

    int num_vertices;
    std::vector< std::vector< int > > graph;
    
    int a_size, b_size;
    std::vector<int> pair_A, pair_B;
    std::vector<int> dist;
};

#endif