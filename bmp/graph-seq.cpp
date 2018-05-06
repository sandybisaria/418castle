#include <climits>
#include <iostream>
#include <queue>

#include "graph-seq.h"

#define DUMMY (num_vertices)
#define SD(i) ((i) - a_size)
#define SU(i) ((i) + a_size)

Graph::Graph(int num_vertices, int b){
    graph.assign(num_vertices, std::vector<int>(num_vertices + 1, 0));
    this->num_vertices = num_vertices;

    a_size = b;
    b_size = num_vertices - b;
    pair_A.assign(a_size, DUMMY);
    pair_B.assign(b_size, DUMMY);
    dist.assign(num_vertices + 1, 0);

    //TODO: Initialize dummy vertex?
} 

void Graph::addEdges(int u, int v, int w) {
    graph[u][v] = w;
    graph[v][u] = w;
}

bool Graph::bfs() {
    std::queue<int> q;

    for (int i_a = 0; i_a < a_size; i_a++) {
        if (pair_A[i_a] == DUMMY) {
            dist[i_a] = 0;
            q.push(i_a);
        } else {
            dist[i_a] = INT_MAX;
        }
    }

    dist[DUMMY] = INT_MAX;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        if (dist[u] < dist[DUMMY]) {
            for (int v = 0; v < b_size; v++) {
                if (graph[u][SU(v)] == 1 && dist[pair_B[v]] == INT_MAX) {
                    dist[pair_B[v]] = dist[u] + 1;
                    q.push(pair_B[v]);
                }
            }
        }
    }

    return dist[DUMMY] != INT_MAX;
}

bool Graph::dfs(int u) {
    if (u == DUMMY) {
        return true;
    }

    for (int v = 0; v < b_size; v++) {
        if (graph[u][SU(v)] == 1 && dist[pair_B[v]] == dist[u] + 1 && dfs(pair_B[v])) {
            pair_B[v] = u;
            pair_A[u] = v;
            return true;
        }
    }

    dist[u] = INT_MAX;
    return false;
}

int Graph::bipartiteMatching() {
    int matching = 0;

    while (bfs()) {
        for (int i_a = 0; i_a < a_size; i_a++) {
            if (pair_A[i_a] == DUMMY && dfs(i_a)) {
                matching++;
            }
        }
    }

    return matching;
}