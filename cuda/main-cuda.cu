#include <stdio.h>

#include <cuda.h>
#include <cuda_runtime.h>
#include <driver_functions.h>

#include "CycleTimer.h"
#include "graph-cuda.h"

__global__ 
void findPath(int* E, int* V, int* F, int* visited, int* parentP, int* flag, int* G, int* num_vertices){
    int id = threadIdx.x + blockIdx.x * blockDim.x;
    if (id > (*num_vertices)){
        *flag = 0;
        return;
    }

    printf("id = %d F[%d] = %d\n",id,id,F[id]);

    if (F[id] == 1 && visited[id] == 0)
    {
        F[id] = 0;
        visited[id] = 1;
        __syncthreads(); 
        int start = V[id];
        int end = V[id+1];
        printf("%d to %d\n", start, end);
        for (int i = start; i < end; i++) 
        {
            int nid = E[i];
            printf("edge from %d to %d\n", id, nid);
            printf("weight = %d\n", G[(id * (*num_vertices)) + nid]);
            if (visited[nid] == 0 && (G[(id * (*num_vertices)) + nid] > 0))
            {
                parentP[nid] = id;
                F[nid] = 1;
                *flag = 0;
            }
        }
    }
}


long Graph::maxFlow(int s, int t){
    long max_flow = 0;

    int *parentP_cuda;
    int *parentP_host = (int *)calloc(num_vertices, sizeof(int));
    cudaMalloc(&parentP_cuda, sizeof(int) * num_vertices);
    cudaMemset(parentP_cuda, 0, sizeof(int) * num_vertices);

    int* graph_cuda;
    cudaMalloc(&graph_cuda, sizeof(int) * num_vertices * num_vertices);
    for (int row = 0; row < num_vertices; row++)
        cudaMemcpy(&(graph_cuda[row * num_vertices]), graph[row].data(), 
            num_vertices, cudaMemcpyHostToDevice);

    int* E;
    cudaMalloc(&E, sizeof(int) * num_edges);
    cudaMemcpy(E, edges.data(), sizeof(int) * num_edges,
        cudaMemcpyHostToDevice);

    int* V;
    cudaMalloc(&V, sizeof(int)*(num_vertices + 1));
    cudaMemcpy(V, nodes.data(), sizeof(int) * (num_vertices + 1),
        cudaMemcpyHostToDevice);

    int* F;
    cudaMalloc(&F, sizeof(int) * num_vertices);
    cudaMemset(F, 0, sizeof(int) * num_vertices);
    cudaMemset(&(F[s]), 1 , sizeof(int));
    
    int* visited_cuda;
    int* visited_host = (int *)calloc(num_vertices, sizeof(int));
    cudaMalloc(&visited_cuda, sizeof(int) * num_vertices);
    cudaMemset(visited_cuda, 0, sizeof(int) * num_vertices);

    int *flag_cuda;
    int *flag_host;
    flag_host = (int *)calloc(1, sizeof(int));
    cudaMalloc(&flag_cuda, sizeof(int));
    cudaMemset(flag_cuda, 0, sizeof(int));

    int* nv;
    cudaMalloc(&nv, sizeof(int));
    cudaMemcpy(nv, &num_vertices, sizeof(int), cudaMemcpyHostToDevice);

    int blocks = 1;
    int threadsPerBlock = 32;

    int isPath = 0;

    *flag_host = 0;
    while (*flag_host == 0){
        *flag_host = 1;
        cudaMemcpy(flag_cuda, flag_host, sizeof(int), cudaMemcpyHostToDevice);
        findPath <<<blocks, threadsPerBlock >>>(E, V, F, visited_cuda,
            parentP_cuda, flag_cuda, graph_cuda, nv);
        // for (int x = 0; x < num_vertices; x++){
        //     printf("%d\n", visited_host[x]);
        // }
        cudaMemcpy(flag_host, flag_cuda, sizeof(int), cudaMemcpyDeviceToHost);
    }

    cudaMemcpy(parentP_host, parentP_cuda, num_vertices * sizeof(int),
        cudaMemcpyDeviceToHost);
    cudaMemcpy(visited_host, visited_cuda, num_vertices * sizeof(int),
        cudaMemcpyDeviceToHost);

    

    isPath = (visited_host[t] == 1);

    while (isPath){
        long flow = LONG_MAX;
        for (int v = t; v != s; v = parentP_host[v]){
            int u = parentP_host[v];
            flow = std::min(flow, (long) graph[u][v]);
        }
        for (int v = t; v != s; v = parentP_host[v]){
            int u = parentP_host[v];
            graph[u][v] -= flow;
            graph[v][u] += flow;
        }
        max_flow += flow;

        cudaMemset(parentP_cuda, 0, sizeof(int) * num_vertices);
        for (int row = 0; row < num_vertices; row++)
            cudaMemcpy(&(graph_cuda[row * num_vertices]), graph[row].data(), 
                num_vertices, cudaMemcpyHostToDevice);
        cudaMemset(F, 0, num_vertices);
        cudaMemset(&F[s], 1 , sizeof(int));
        cudaMemset(&visited_cuda, 0, num_vertices * sizeof(int));

        *flag_host = 0;
        while (*flag_host == 0){
            *flag_host = 1;
            cudaMemcpy(&flag_cuda, &flag_host, sizeof(int),
                cudaMemcpyHostToDevice);
            findPath <<<blocks, threadsPerBlock >>>(E, V, F, visited_cuda,
                parentP_cuda, flag_cuda, graph_cuda, nv);
            cudaMemcpy(&flag_host, &flag_cuda , sizeof(int), cudaMemcpyDeviceToHost);
        }

        cudaMemcpy(parentP_host, parentP_cuda, num_vertices * sizeof(int),
            cudaMemcpyDeviceToHost);
        cudaMemcpy(visited_host, visited_cuda, num_vertices * sizeof(int),
            cudaMemcpyDeviceToHost);

        isPath = (visited_host[t] == 1);
    }
    return max_flow;
}