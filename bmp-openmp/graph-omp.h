#ifndef GRAPH_SEQ_H
#define GRAPH_SEQ_H

#include <vector>
#include <set>

class Graph {
public:
    Graph(int num_vertices);
    void addEdges(int u, int v, int w);
    long parallelKarpSipser();
    int getDegree(int u);
    void matchAndUpdate(std::set<std::tuple<int, int>>& M,std::vector<int>& visited, int u);

private:
    int num_vertices;
    std::vector< std::vector< int > > graph;
    std::vector< int > X;
    std::vector< int > Y;
    int sizeX;
    int sizeY;

};

#endif