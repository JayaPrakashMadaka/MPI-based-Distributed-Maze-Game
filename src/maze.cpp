#include <iostream>
#include <string>
#include <cstring>
#include "mpi.h"
#include "generator/mazegenerator.hpp"
#include "solver/mazesolver.hpp"
using namespace std;

int main(int argc,char* argv[]){
    MPI_Init(NULL, NULL);
    if(strcmp(argv[2],"bfs") == 0 && strcmp(argv[4],"dfs") == 0){
        mazegenerator(0);
        MPI_Barrier(MPI_COMM_WORLD);
        mazesolver(0,"src/MAZE_BFS.txt");
    }
    else if(strcmp(argv[2],"bfs") == 0 && strcmp(argv[4],"dijkstra") == 0){
        mazegenerator(0);
        MPI_Barrier(MPI_COMM_WORLD);
        mazesolver(1,"src/MAZE_BFS.txt");
    }
    else if(strcmp(argv[2],"kruskal") == 0 && strcmp(argv[4],"dfs") == 0){
        mazegenerator(1);
        MPI_Barrier(MPI_COMM_WORLD);
        mazesolver(0,"src/MAZE_KRUSKAL.txt");
    }
    else if(strcmp(argv[2],"kruskal") == 0 && strcmp(argv[4],"dijkstra") == 0){
        mazegenerator(1);
        MPI_Barrier(MPI_COMM_WORLD);
        mazesolver(1,"src/MAZE_KRUSKAL.txt");
    }
    MPI_Finalize();
    return 0;
}
