#include <climits>
#include <cassert>
#include <iostream>
#include <queue>

#include "graph-mpi.h"

static int start_position(int N, int P, int i) {
    int base = N / P;
    int extra = N % P;
    if (i < extra)
        return i * (base + 1);
    else
        return i * base + extra;
}

Graph::Graph(int num_vertices) {
    graph.assign(num_vertices, std::vector<int>(num_vertices, 0));
    this->num_vertices = num_vertices;
} 

void Graph::addEdges(int u, int v, int w){
    graph[u][v] = w;
}

void Graph::partition(int s, int process_count, std::vector< std::vector<bool> >& vertices_per_proc) {
    std::vector<bool> visited(num_vertices, false);
    std::queue<int> q;
    q.push(s);
    visited[s] = true;

    int nodes_added = 0;
    int id = 0;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        vertices_per_proc[id][u] = true;
        nodes_added++;
        if (nodes_added >= start_position(num_vertices, process_count, id + 1)) {
            id++;
        }

        for (int v = 0; v < num_vertices; v++){
            if (visited[v] == false && graph[u][v] > 0) {
                q.push(v);
                visited[v] = true;
            }
        }
    }

    // if (nodes_added != num_vertices) {
    //     std::cout << nodes_added << " " << num_vertices << std::endl;
    // } else {
    //     for (int pid = 0; pid < process_count; pid++) {
    //         for (int nid = 0; nid < num_vertices; nid++) {
    //             std::cout << vertices_per_proc[pid][nid];
    //         }
    //         std::cout << std::endl;
    //     }
    // }
}

long Graph::maxFlow(int s, int t){
    int process_count = 0;
    int process_id = 0;

    MPI_Comm_size(MPI_COMM_WORLD, &process_count);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_id);

    assert(process_id == 0); // Only the master should be here!

    std::vector< std::vector<bool> > vertices_per_proc;
    vertices_per_proc.assign(process_count, std::vector<bool>(num_vertices, false));

    partition(s, process_count, vertices_per_proc);

    return 0;
}



