#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include "mpi.h"
#include "dijkstra.hpp"
#include "mazesolver.hpp"
using namespace std;

bool solveMaze_dijkstra(int rows, int cols, int* maze, int* path,pair<int, int> P0,pair<int, int> P1) {
    queue<pair<int, int> > q;
    q.push(make_pair(P0.first, P0.second));
    path[P0.first*cols + P0.second] = 1;
    int targetX = P1.first;
    int targetY = P1.second;
    while (!q.empty()) {
        int x = q.front().first;
        int y = q.front().second;
        q.pop();
        if (x == targetX && y == targetY)
            return true;
        vector<pair<int, int> > neighbors = {{x - 1, y}, {x + 1, y}, {x, y - 1}, {x, y + 1}};
        for (auto& neighbor : neighbors) {
            int newX = neighbor.first;
            int newY = neighbor.second;
            if (isValid(newX, newY, rows, cols) && maze[newX*cols + newY] != WALL && path[newX*cols + newY] == 0) {
                path[newX*cols + newY] = path[x*cols + y] + 1;
                q.push(neighbor);
            }
        }
    }
    return false;
}

void mazesolver_child_dijkstra(int rows,int cols,pair<int,int> p0,pair<int,int> p1,int* maze){
    
    int* path = (int*)malloc(rows*cols*sizeof(int));
    for(int i=0;i<rows*cols;i++)path[i]=0;

    bool solvable = solveMaze_dijkstra(rows,cols,maze,path,p0,p1);
    if(!solvable){
        cerr<<"Maze Not Solvable !"<<endl;
        return;
    }
    stack<pair<int,int>> st;
    st.push(p1);
    while(path[st.top().first*cols + st.top().second]!=1){
        int x=st.top().first;
        int y=st.top().second;
        vector<pair<int, int>> neighbors = {{x - 1, y}, {x + 1, y}, {x, y - 1}, {x, y + 1}};
        int min_x=-1;
        int min_y=-1;
        int min_val=path[x*cols + y];
        for (auto& neighbor : neighbors) {
            int newX = neighbor.first;
            int newY = neighbor.second;
            if (isValid(newX, newY, rows, cols) && path[newX*cols+newY] < min_val && path[newX*cols + newY] != 0) {
                min_val=path[newX*cols + newY];
                min_x=newX;
                min_y=newY;
            }
        }
        st.push(make_pair(min_x,min_y));
    }

    while(!st.empty()){
        int x=st.top().first;
        int y=st.top().second;
        st.pop();
        maze[x*cols+y] = PATH;
    }
    maze[p0.first*cols + p0.second]=START;
    maze[p1.first*cols + p1.second]=END;
}

void mazesolver_dijkstra(int n,int m,string filename){
    int num_procs,rank;
    MPI_Request send_req,recv_req;
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int t = n/num_procs;

    int* maze = (int*)malloc(n*m*sizeof(int));
    for(int i=0;i<n*m;i++)maze[i]=0;
    ifstream input(filename);
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            input>>maze[i*m+j];
        }
    }
    input.close();
    int* partial_maze = (int*)malloc(n*m*sizeof(int));
    for(int i=0;i<n*m;i++)partial_maze[i]=0;
    for(int i=0;i<t;i++){
        for(int j=0;j<m;j++){
            partial_maze[i*m+j] = maze[rank*t*m + i*m+ j];
        }
    }
    int start_point;
    if(rank != 0){
        for(int j=0;j<m;j++){
            if(partial_maze[j]==WAY){
                start_point = j;
                MPI_Isend(&start_point, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD,&send_req);
                break;
            }
        }
    }
    if(rank==0){
        start_point = m-1;
    }
    int end_point;
    if(rank != 3){
        MPI_Irecv(&end_point, 1, MPI_INT,rank+1, 0, MPI_COMM_WORLD, &recv_req);
        MPI_Wait(&recv_req, MPI_STATUS_IGNORE);
    }
    if(rank == 3) end_point = 0;

    mazesolver_child_dijkstra(t,m,make_pair(0,start_point),make_pair(t-1,end_point),partial_maze);
    if(rank != 3){
        partial_maze[(t-1)*m + end_point]=PATH;
    }
    if(rank != 0){
        partial_maze[start_point]=PATH;
    }

    if(rank != 0){
        MPI_Send(partial_maze, t * m, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    if(rank == 0){
        int* maze1 = (int*)malloc(t * m * sizeof(int));
        int* maze2 = (int*)malloc(t * m * sizeof(int));
        int* maze3 = (int*)malloc(t * m * sizeof(int));

        MPI_Recv(maze1, t * m , MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(maze2, t * m , MPI_INT, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(maze3, t * m , MPI_INT, 3, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        for(int i=0;i<t;i++){
            for(int j=0;j<m;j++){
                cout << val(partial_maze[i*m+j]) << " ";
            }
            cout<<endl;
        }
        for(int i=0;i<t;i++){
            for(int j=0;j<m;j++){
                cout << val(maze1[i*m+j]) << " ";
            }
            cout<<endl;
        }
        for(int i=0;i<t;i++){
            for(int j=0;j<m;j++){
                cout << val(maze2[i*m+j]) << " ";
            }
            cout<<endl;
        }
        for(int i=0;i<t;i++){
            for(int j=0;j<m;j++){
                cout << val(maze3[i*m+j]) << " ";
            }
            cout<<endl;
        }
    }
}
