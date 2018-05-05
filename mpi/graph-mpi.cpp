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
    int process_count = 0;
    int process_id = 0;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &process_count);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_id);

    bool mpi_master = process_id == 0;

    if (mpi_master){
        
    }


    return 0;
}



