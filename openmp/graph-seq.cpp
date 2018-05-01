#include <climits>
#include <iostream>
#include <queue>

#include "graph-seq.h"

Graph::Graph(int num_vertices){
    graph.assign(num_vertices, std::vector<int>(num_vertices, 0));
    this->num_vertices = num_vertices;
}

void Graph::addEdges(int u, int v, int w){
    graph[u][v] = w;
}

long Graph::maxFlow(int s, int t) {
    long *vertex_height = (long *)calloc(num_vertices, sizeof(long));
    vertex_height[s] = num_vertices;

    long *net_flow = (long *)calloc(num_vertices, sizeof(long));
    long *residual_cap = (long *)calloc(num_vertices * num_vertices, sizeof(long));
    for (int i = 0; i < num_vertices; i++)
        for (int j = 0; j < num_vertices; j++)
            residual_cap[i*num_vertices + j] = graph[i][j];

    for (int u = 0; u < num_vertices; u++) {
        residual_cap[s*num_vertices + u] = 0;
        residual_cap[u*num_vertices + s] = graph[u][s] + graph[s][u];
        net_flow[u] = graph[s][u];
    }

    while (1) {
        bool operations_done = false;
        for (int u = 0; u < num_vertices; u++) {
            if (u == s || u == t)
                continue;

            while (net_flow[u] > 0) {
                operations_done = true;

                long old_flow = net_flow[u];
                int lowest_neighbor = -1;
                long lowest_height = LONG_MAX;
                for (int v = 0; v < num_vertices; v++) {
                    if (residual_cap[u*num_vertices + v] > 0) {
                        if (vertex_height[v] < lowest_height) {
                            lowest_neighbor = v;
                            lowest_height = vertex_height[v];
                        }
                    }
                }

                if (vertex_height[u] > lowest_height) {
                    long d = old_flow;
                    if (residual_cap[u*num_vertices + lowest_neighbor] < d)
                        d = residual_cap[u*num_vertices + lowest_neighbor];

                    residual_cap[u*num_vertices + lowest_neighbor] -= d;
                    residual_cap[lowest_neighbor*num_vertices + u] += d;

                    net_flow[u] -= d;
                    net_flow[lowest_neighbor] += d;
                } else {
                    vertex_height[u] = lowest_height + 1;
                }
            }
        }

        if (!operations_done)
            break;
    }

    // for (int u = 0; u < num_vertices; u++) {
    //     printf("net_flow[%d] = %ld\n", u, net_flow[u]);
    // }
    // for (int u = 0; u < num_vertices; u++) {
    //     printf("vertex_height[%d] = %ld\n", u, vertex_height[u]);
    // }
    // for (int u = 0; u < num_vertices; u++) {
    //     for (int v = 0; v < num_vertices; v++) {
    //         printf("residual_cap[%d][%d] = %ld\n", u, v, residual_cap[u]);
    //     }
    // }

    // long max_flow = 0;
    // for (int u = 0; u < num_vertices; u++) {
    //     max_flow += net_flow[u];
    // }
    return net_flow[t];
}

