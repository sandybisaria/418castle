#include <iostream> 

#include "graph-seq.h"

using namespace std;
 
int main(int argc, char** argv) {
    Graph graph(6);
    graph.addEdges(0,1,11);
    graph.addEdges(0,2,12);
    graph.addEdges(2,1,1);
    graph.addEdges(1,3,12);
    graph.addEdges(2,4,11);
    graph.addEdges(4,3,7);
    graph.addEdges(3,5,19);
    graph.addEdges(4,5,4);
    cout << graph.maxFlow(0,5)<< endl;
    return 0;
}