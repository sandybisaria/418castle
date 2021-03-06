#include <iostream> 
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string>

#include "cycletimer.h"
#include "graph-seq.h"

using namespace std;
 
int main(int argc, char* argv[]) {
    if (argc < 3) { 
        cout << "Usage: -g <graph.txt>"<< endl; 
        exit(0);
    }
    else{
        char* FILE = NULL;
        string flag = "-g";
        if (argv[1] == flag){
            FILE = argv[2];
        }
        if (FILE == NULL){
            cout << "Please enter a valid graph.txt file>" << endl; 
            cout << "Usage: -g <graph.txt>" << endl; 
            exit(0);
        }
        else{
            ifstream file(FILE);
            string data;
            int num_vertices, s, t;
            long max_flow_expected, max_flow;
            num_vertices = 0;
            s = 0;
            t = 0;
            max_flow_expected = 0;
            getline(file, data);
            num_vertices = stoi(data);
            if (num_vertices == 0){
                cout << "Error: vertices = 0" << endl;
                exit(0);
            }
            Graph graph(num_vertices);
            while (file && getline(file, data)){
                vector<string> array;
                size_t pos = 0, found;
                while((found = data.find_first_of(' ',pos))!= string::npos){
                    array.push_back(data.substr(pos, found - pos));
                    pos = found+1;
                }
                array.push_back(data.substr(pos));
                if (array.size() == 0){
                    continue;
                }
                else if (array.size() == 1){
                    max_flow_expected = stol(array[0]);
                }
                else if (array.size() == 2){
                    s = stoi(array[0]);
                    t = stoi(array[1]);
                }
                else if (array.size() == 3){
                    graph.addEdges(stoi(array[0]),stoi(array[1]),
                                                        stoi(array[2]));
                }
            }
            cout << "Testing with input graph file = " << FILE << endl; 
            double startTime = currentSeconds();
            max_flow = graph.maxFlow(s,t);
            double endTime = currentSeconds();
            if (max_flow == max_flow_expected){
                cout << "Passed" << endl;
                cout << endTime - startTime << endl;
            }
            else{
                cout << "Failed" << endl; 
                cout << "Flow returned = " << max_flow << " " << 
                "Flow expected = " << max_flow_expected << endl;
            }
            return 0;
        }
    }
}