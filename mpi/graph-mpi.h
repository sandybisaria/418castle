#ifndef GRAPH_SEQ_H
#define GRAPH_SEQ_H

#include <vector>
#include <mpi.h>

class Graph {
public:
    Graph(int num_vertices);
    void addEdges(int u, int v, int w);
    long maxFlow(int s, int t);

private:
    void partition(int s, int process_count, std::vector< std::vector<bool> >& vertices_per_proc);
    int num_vertices;
    std::vector< std::vector< int > > graph;
};

#endif