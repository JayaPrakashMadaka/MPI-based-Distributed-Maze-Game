#include <stack>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono> 
#include <iostream>
#include <fstream>
#include <mpi.h>
#include "kruskal.hpp"
#include "mazegenerator.hpp"
using namespace std;


void mazegenerator_kruskal(int n,int m) {
    srand(time(0));
    int num_procs, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int t = n/num_procs;
    
    
    pair<int,int> start_point;
    pair<int,int> end_point;
    int start_val;
    int end_val;

    if(rank == 0){
        int end_point1 = getRandomInt(m);
        int end_point2 = getRandomInt(m);
        int end_point3 = getRandomInt(m);
        start_point = make_pair(0,m-1);
        end_point = make_pair(t-1,end_point1);
        MPI_Send(&end_point1, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Send(&end_point2, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Send(&end_point2, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
        MPI_Send(&end_point3, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
        MPI_Send(&end_point3, 1, MPI_INT, 3, 0, MPI_COMM_WORLD);
        start_val = 2;
        end_val = 1;

    }
    else if(rank == 1 || rank == 2){
        int start_pointbuf;
        int end_pointbuf;
        MPI_Recv(&start_pointbuf, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&end_pointbuf, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        start_point = make_pair(1,start_pointbuf);
        end_point = make_pair(t-1,end_pointbuf);
        start_val = 1;
        end_val = 1;
    }
    else{
        int start_pointbuf;
        MPI_Recv(&start_pointbuf,1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        start_point = make_pair(1,start_pointbuf);
        end_point = make_pair(t-1,0);
        start_val = 1;
        end_val = 3;
    }

    int* partial_maze = (int*)malloc(t * m * sizeof(int));
    for(int i=0;i<t*m;i++)partial_maze[i]=0;
    random_maze_generator_kruskal(t,m,start_point,end_point,start_val,end_val,partial_maze);

    if(rank != 0){
        partial_maze[start_point.second]=1;
        MPI_Send(partial_maze, t * m, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    else{
        int* maze1 = (int*)malloc(t * m * sizeof(int));
        int* maze2 = (int*)malloc(t * m * sizeof(int));
        int* maze3 = (int*)malloc(t * m * sizeof(int));

        MPI_Recv(maze1, t * m , MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(maze2, t * m , MPI_INT, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(maze3, t * m , MPI_INT, 3, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        ofstream outFile("src/MAZE_KRUSKAL.txt");

        
        for(int i=0;i<t;i++){
            for(int j=0;j<m;j++){
                outFile << partial_maze[i*m+j] << " ";
            }
            outFile<<endl;
        }
        for(int i=0;i<t;i++){
            for(int j=0;j<m;j++){
                outFile << maze1[i*m+j] << " ";
            }
            outFile<<endl;
        }
        for(int i=0;i<t;i++){
            for(int j=0;j<m;j++){
                outFile << maze2[i*m+j] << " ";
            }
            outFile<<endl;
        }
        for(int i=0;i<t;i++){
            for(int j=0;j<m;j++){
                outFile << maze3[i*m+j] << " ";
            }
            outFile<<endl;
        }
        
        outFile.close();

        free(maze1);
        free(maze2);
        free(maze3);
        free(partial_maze);
    }
}
