#include <climits>
#include <iostream>
#include <queue>
#include <tuple>
#include <omp.h>
#include <atomic>

#include "graph-omp.h"

#define SD(i) ((i) - a_size)
#define SU(i) ((i) + a_size)

Graph::Graph(int num_vertices, int b){
    this->num_vertices = num_vertices;
    graph.assign(num_vertices, std::vector<int>(num_vertices, 0));
    degree.assign(num_vertices, 0);
}

void Graph::addEdges(int u, int v, int w) {
    graph[u][v] = w;
    graph[v][u] = w;

    degree[u]++;
    degree[v]++;
}

int Graph::bipartiteMatching() {
    return 0;
}

AdjMat Graph::karpSipser() {
    AdjMat matching(num_vertices, std::vector<int>(num_vertices, 0));

    std::vector<int> Q_1;
    Q_1.reserve(num_vertices);
    std::vector<int> Q_star;
    Q_star.reserve(num_vertices);

    std::vector<bool> visited;
    visited.assign(num_vertices, false); // Parallelize?

    // Parallelize?
    for (int a = 0; a < a_size; a++) {
        if (degree[a] == 1) {
            Q_1.push_back(a);
        } else {
            Q_star.push_back(a);
        }
    }

    // Parallelize?
    for (auto a : Q_1) {
        ;
    }

    // Parallelize?
    for (auto a : Q_star) {
        ;
    }

    return matching;
}

void Graph::matchAndUpdate(AdjMat& matching, int a, std::vector<bool>& visited) {
    if (std::atomic_fetch_add(&visited[a], 1) == 0) {
        for (int b = b_size; b < num_vertices; b++) {
            if (graph[a][b] != 1)
                continue;

            if (std::atomic_fetch_add(&visited[b], 1) == 0) {
                matching[a][b] = 1;
                matching[b][a] = 1;

                
            }
        }
    }
}