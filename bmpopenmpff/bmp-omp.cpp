#include <iostream> 
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <omp.h>

#include "cycletimer.h"
#include "graph-omp.h"

using namespace std;
 
int main(int argc, char* argv[]) {
    if (argc < 3) { 
        std::cout << "Usage: -g <graph.txt>"<< endl; 
        exit(0);
    }
    else{
        char* FILE = NULL;
        string flag = "-g";
        if (argv[1] == flag){
            FILE = argv[2];
        }
        if (FILE == NULL){
            std::cout << "Please enter a valid graph.txt file>" << endl; 
            std::cout << "Usage: -g <graph.txt>" << endl; 
            exit(0);
        }
        else{
            int num_vertices = 0;
            int second_partition = 0;
            int bpm_expected = 0;

            ifstream file(FILE);
            string data;
            Graph graph(0, 0); // Should be replaced...
            while (file && getline(file, data)){
                std::vector<std::string> array;
                std::size_t pos = 0, found;
                while((found = data.find_first_of(' ',pos))!= std::string::npos){
                    array.push_back(data.substr(pos, found - pos));
                    pos = found+1;
                }
                array.push_back(data.substr(pos));
                if (array.size() == 0){
                    continue;
                }
                else if (array.size() == 1){
                    bpm_expected = std::stol(array[0]);
                }
                else if (array.size() == 2){
                    num_vertices = std::stoi(array[0]);
                    second_partition = std::stoi(array[1]);

                    graph = Graph(num_vertices, second_partition);
                }
                else if (array.size() == 3){
                    graph.addEdges(std::stoi(array[0]), std::stoi(array[1]), std::stoi(array[2]));
                }
            }
            omp_set_num_threads(8);
            std::cout << "Testing with input graph file = " << FILE << endl; 
            double startTime = currentSeconds();
            int bpm = graph.bipartiteMatching();
            double endTime = currentSeconds();
            if (bpm == bpm_expected){
                std::cout << "Passed" << endl;
                std::cout << endTime - startTime << endl;
            }
            else{
                std::cout << "Failed" << endl; 
                std::cout << "Matching size found = " << bpm << " " << 
                             "Matching size expected = " << bpm_expected << endl;
            }
            return 0;
        }
    }
}