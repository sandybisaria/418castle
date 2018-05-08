#include <climits>
#include <iostream>
#include <queue>
#include <tuple>
#include <omp.h>
#include <atomic>

#include "graph-omp.h"

#define SOURCE (num_vertices) // graph[num_vertices] is source
#define SINK (num_vertices+1) // graph[num_vertices+1] is sink
#define SD(i) ((i) - a_size)
#define SU(i) ((i) + a_size)

static int start_position(int N, int P, int i) {
    int base = N / P;
    int extra = N % P;
    if (i < extra)
        return i * (base + 1);
    else
        return i * base + extra;
}

Graph::Graph(int num_vertices, int b){
    this->num_vertices = num_vertices;
    graph.assign(num_vertices+2, std::vector<int>(num_vertices+2, 0));

    a_size = b;
    b_size = num_vertices - b;
}

void Graph::addEdges(int u, int v, int w) {
    graph[u][v] = w;
}

int Graph::bipartiteMatching() {
    for (int i = 0; i < a_size; i++) {
        graph[SOURCE][i] = 1;
    }
    for (int i = 0; i < b_size; i++) {
        graph[SU(i)][SINK] = 1;
    }

    // Solve maximum flow on this graph
    return maxFlow(SOURCE, SINK);
}

long Graph::maxFlow(int s, int t){
    num_vertices += 2; // Account for source and sink... (temp fix)
    long *vertex_height = (long *)calloc(num_vertices, sizeof(long));
    vertex_height[s] = num_vertices;

    long *net_flow = (long *)calloc(num_vertices, sizeof(long));
    long *residual_cap = (long *)calloc(num_vertices * num_vertices, sizeof(long));
    for (int i = 0; i < num_vertices; i++)
        for (int j = 0; j < num_vertices; j++)
            residual_cap[i*num_vertices + j] = graph[i][j];

    long s_outflow = 0;
    for (int u = 0; u < num_vertices; u++) {
        residual_cap[s*num_vertices + u] = 0;
        residual_cap[u*num_vertices + s] = graph[u][s] + graph[s][u];
        net_flow[u] = graph[s][u];
        s_outflow += graph[s][u];
    }

    #pragma omp parallel shared(vertex_height, net_flow, residual_cap, s_outflow)
    {
        int start = start_position(num_vertices, omp_get_num_threads(), omp_get_thread_num());
        int end = start_position(num_vertices, omp_get_num_threads(), omp_get_thread_num() + 1);

        while (s_outflow != net_flow[t]) {
            for (int u = start; u < end; u++) {
                if (u == s || u == t)
                    continue;

                while (net_flow[u] > 0) {

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

                            __sync_fetch_and_sub(&residual_cap[u*num_vertices + lowest_neighbor], d);
                            __sync_fetch_and_add(&residual_cap[lowest_neighbor*num_vertices + u], d);
                            __sync_fetch_and_sub(&net_flow[u], d);
                            __sync_fetch_and_add(&net_flow[lowest_neighbor], d);

                            if (lowest_neighbor == s)
                                __sync_fetch_and_sub(&s_outflow, d);
                    } else {
                        vertex_height[u] = lowest_height + 1;
                    }
                }
            }
        }
    }

    long max_flow = 0;
    for (int u = 0; u < num_vertices; u++) {
        if (u != s)
            max_flow += net_flow[u];
    }

    free(vertex_height);
    free(net_flow);
    free(residual_cap);

    return max_flow;
}
