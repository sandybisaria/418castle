#include <stdio.h>

#include <cuda.h>
#include <cuda_runtime.h>
#include <driver_functions.h>

#include "CycleTimer.h"
#include "graph-cuda.h"

__global__ 
void findPath(int *E, int *V, int *F, int *visited, int *parentP, int *flag,
    int *G, int num_vertices) {

    int id = threadIdx.x + blockIdx.x * blockDim.x;
    if (id >= num_vertices) {
        // *flag = 0;
        return;
    }

    // printf("id = %d F[%d] = %d\n", id, id, F[id]);
    if (F[id] != 0 && visited[id] == 0) {
        F[id] = 0;
        visited[id] = 1;
        __syncthreads(); 
        int start = V[id];
        int end = V[id+1];
        // printf("%d to %d\n", start, end);
        for (int i = start; i < end; i++) {
            int nid = E[i];
            // printf("edge from %d to %d\n", id, nid);
            // printf("weight = %d\n", G[(id * num_vertices) + nid]);
            if (visited[nid] == 0 && G[(id * num_vertices) + nid] > 0) {
                parentP[nid] = id;
                F[nid] = 1;
                *flag = 0;
            }
        }
    }
}


long Graph::maxFlow(int s, int t) {
    long max_flow = 0;

    const size_t VERTEX_ROW_SIZE = sizeof(int) * num_vertices;

    int *parentP_host = (int *)malloc(VERTEX_ROW_SIZE);
    int *parentP_cuda;
    cudaMalloc(&parentP_cuda, VERTEX_ROW_SIZE);
    cudaMemset(parentP_cuda, 0, VERTEX_ROW_SIZE);

    int *graph_cuda;
    cudaMalloc(&graph_cuda, VERTEX_ROW_SIZE * num_vertices);
    for (int row = 0; row < num_vertices; row++)
        cudaMemcpy(&graph_cuda[row * num_vertices], graph[row].data(), 
            VERTEX_ROW_SIZE, cudaMemcpyHostToDevice);

    int *E;
    cudaMalloc(&E, sizeof(int) * num_edges);
    cudaMemcpy(E, edges.data(), sizeof(int) * num_edges, cudaMemcpyHostToDevice);

    int *V;
    cudaMalloc(&V, VERTEX_ROW_SIZE + sizeof(int));
    cudaMemcpy(V, nodes.data(), VERTEX_ROW_SIZE + sizeof(int), cudaMemcpyHostToDevice);

    int *F_temp = (int *)calloc(num_vertices, sizeof(int));
    F_temp[s] = 1;
    int *F;
    cudaMalloc(&F, VERTEX_ROW_SIZE);
    cudaMemcpy(F, F_temp, VERTEX_ROW_SIZE, cudaMemcpyHostToDevice);
    
    int *visited_host = (int *)malloc(VERTEX_ROW_SIZE);
    int *visited_cuda;
    cudaMalloc(&visited_cuda, VERTEX_ROW_SIZE);
    cudaMemset(visited_cuda, 0, VERTEX_ROW_SIZE);

    int *flag_host = (int *)malloc(sizeof(int));
    int *flag_cuda;
    cudaMalloc(&flag_cuda, sizeof(int));
    cudaMemset(flag_cuda, 0, sizeof(int));

    int threadsPerBlock = 32;
    int blocks = 1 + (num_vertices / threadsPerBlock);

    *flag_host = 0;
    while (*flag_host == 0) {
        *flag_host = 1;
        cudaMemcpy(flag_cuda, flag_host, sizeof(int), cudaMemcpyHostToDevice);
        findPath<<<blocks, threadsPerBlock >>>(E, V, F, visited_cuda,
            parentP_cuda, flag_cuda, graph_cuda, num_vertices);
        cudaMemcpy(flag_host, flag_cuda, sizeof(int), cudaMemcpyDeviceToHost);
    }
    cudaMemcpy(parentP_host, parentP_cuda, num_vertices * sizeof(int),
        cudaMemcpyDeviceToHost);
    cudaMemcpy(visited_host, visited_cuda, num_vertices * sizeof(int),
        cudaMemcpyDeviceToHost);
    int isPath = (visited_host[t] != 0);

    while (isPath) {
        long flow = LONG_MAX;
        for (int v = t; v != s; v = parentP_host[v]){
            int u = parentP_host[v];
            flow = std::min(flow, (long) graph[u][v]);
        }
        for (int v = t; v != s; v = parentP_host[v]) {
            int u = parentP_host[v];
            graph[u][v] -= flow;
            graph[v][u] += flow;
        }
        max_flow += flow;

        cudaMemset(parentP_cuda, 0, VERTEX_ROW_SIZE);
        for (int row = 0; row < num_vertices; row++)
            cudaMemcpy(&graph_cuda[row * num_vertices], graph[row].data(), 
                VERTEX_ROW_SIZE, cudaMemcpyHostToDevice);
        cudaMemcpy(F, F_temp, VERTEX_ROW_SIZE, cudaMemcpyHostToDevice);
        cudaMemset(visited_cuda, 0, VERTEX_ROW_SIZE);

        *flag_host = 0;
        while (*flag_host == 0) {
            *flag_host = 1;
            cudaMemcpy(flag_cuda, flag_host, sizeof(int), cudaMemcpyHostToDevice);
            findPath<<<blocks, threadsPerBlock >>>(E, V, F, visited_cuda,
                parentP_cuda, flag_cuda, graph_cuda, num_vertices);
            cudaMemcpy(flag_host, flag_cuda, sizeof(int), cudaMemcpyDeviceToHost);
        }
        cudaMemcpy(parentP_host, parentP_cuda, num_vertices * sizeof(int),
            cudaMemcpyDeviceToHost);
        cudaMemcpy(visited_host, visited_cuda, num_vertices * sizeof(int),
            cudaMemcpyDeviceToHost);
        isPath = (visited_host[t] != 0);
    }
    return max_flow;
}