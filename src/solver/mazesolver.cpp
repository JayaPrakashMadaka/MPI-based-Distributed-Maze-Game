#include <stack>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono> 
#include <iostream>
#include <string>
#include "mazesolver.hpp"
#include "dfs.hpp"
#include "dijkstra.hpp"
using namespace std;

int WALL = 0;
int START = 2;
int END = 3;
int WAY = 1;
int PATH = 4;

bool isValid(int x, int y, int rows, int cols) {
    return x >= 0 && x < rows && y >= 0 && y < cols;
}

string val(int n){
    if(n==0) return "*";
    else if(n==1) return " ";
    else if(n==2) return "S";
    else if(n==3) return "E";
    else return "P";
}

void mazesolver(int type,string filename){
    if(type == 0){
        mazesolver_dfs(64,64,filename);
    }
    else if(type==1){
        mazesolver_dijkstra(64,64,filename);
    }
}