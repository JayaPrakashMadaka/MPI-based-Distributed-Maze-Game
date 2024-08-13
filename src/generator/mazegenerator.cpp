#include <stack>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono> 
#include <iostream>
#include <queue>
#include "mazegenerator.hpp"
#include "bfs.hpp"
#include "kruskal.hpp"
using namespace std;

struct Cell {
    int x, y;
    Cell(int _x, int _y) : x(_x), y(_y) {}
};
void random_maze_generator(int r, int c, pair<int, int> P0, pair<int, int> Pf,int start_val,int end_val,int* maze) {
    const int ROWS = r;
    const int COLS = c;

    bool* seen = (bool*)malloc(ROWS*COLS*sizeof(bool));
    for(int i=0;i<ROWS*COLS;i++)seen[i]=false;
    pair<int,int>* previous = (pair<int,int>*)malloc(ROWS*COLS*sizeof(pair<int,int>));
    for(int i=0;i<ROWS*COLS;i++)previous[i]=make_pair(-1,-1);

    stack<Cell> S;
    S.push(Cell(P0.first, P0.second));
    while (!S.empty()) {
        Cell current = S.top();
        S.pop();
        int x = current.x;
        int y = current.y;
        seen[x * COLS + y] = true;
        if (x + 1 < ROWS && maze[(x + 1)*COLS + y] == 1 && previous[x * COLS + y] != make_pair(x + 1, y)) continue;
        if (x - 1 >= P0.first && maze[(x - 1)*COLS + y] == 1 && previous[x * COLS + y] != make_pair(x - 1, y)) continue;
        if (y + 1 < COLS && maze[x*COLS + y + 1] == 1 && previous[x*COLS + y] != make_pair(x, y + 1)) continue;
        if (y - 1 >= 0 && maze[x*COLS + y - 1] == 1 && previous[x*COLS + y] != make_pair(x, y - 1)) continue;
        maze[x*COLS + y] = 1;
        vector<pair<int, int> > to_stack;
        if (x + 1 < ROWS && !seen[(x + 1)*COLS + y]) {
            seen[(x + 1)*COLS + y] = true;
            to_stack.push_back(make_pair(x + 1, y));
            previous[(x + 1)*COLS + y] = make_pair(x, y);
        }
        if (x - 1 >= P0.first && !seen[(x - 1)*COLS + y]) {
            seen[(x - 1)*COLS + y] = true;
            to_stack.push_back(make_pair(x - 1, y));
            previous[(x - 1)*COLS + y] = make_pair(x, y);
        }
        if (y + 1 < COLS && !seen[x*COLS + y + 1]) {
            seen[x*COLS + y + 1] = true;
            to_stack.push_back(make_pair(x, y + 1));
            previous[x*COLS + y + 1] = make_pair(x, y);
        }
        if (y - 1 >= 0 && !seen[x*COLS + y - 1]) {
            seen[x*COLS + y - 1] = true;
            to_stack.push_back(make_pair(x, y - 1));
            previous[x*COLS + y - 1] = make_pair(x, y);
        }
        random_shuffle(to_stack.begin(), to_stack.end());
        bool pf_flag = false;
        for (pair<int,int>& neighbour : to_stack) {
            if (neighbour == Pf) pf_flag = true;
            else S.push(Cell(neighbour.first, neighbour.second));
        }
        if (pf_flag) S.push(Cell(Pf.first, Pf.second));
    }
    if(seen[Pf.first*COLS + Pf.second]==false){
        return  random_maze_generator(r,c,P0,Pf,start_val,end_val,maze);
    }
    int x0 = P0.first, y0 = P0.second;
    int xf = Pf.first, yf = Pf.second;
    maze[x0*COLS + y0] = start_val;
    maze[xf*COLS + yf] = end_val;
}

void random_maze_generator_kruskal(int r, int c, pair<int, int> P0, pair<int, int> Pf,int start_val,int end_val,int* maze) {
    const int ROWS = r;
    const int COLS = c;

    bool* seen = (bool*)malloc(ROWS*COLS*sizeof(bool));
    for(int i=0;i<ROWS*COLS;i++)seen[i]=false;
    pair<int,int>* previous = (pair<int,int>*)malloc(ROWS*COLS*sizeof(pair<int,int>));
    for(int i=0;i<ROWS*COLS;i++)previous[i]=make_pair(-1,-1);

    stack<Cell> S;
    S.push(Cell(P0.first, P0.second));
    while (!S.empty()) {
        Cell current = S.top();
        S.pop();
        int x = current.x;
        int y = current.y;
        seen[x * COLS + y] = true;
        if (x + 1 < ROWS && maze[(x + 1)*COLS + y] == 1 && previous[x * COLS + y] != make_pair(x + 1, y)) continue;
        if (x - 1 >= P0.first && maze[(x - 1)*COLS + y] == 1 && previous[x * COLS + y] != make_pair(x - 1, y)) continue;
        if (y + 1 < COLS && maze[x*COLS + y + 1] == 1 && previous[x*COLS + y] != make_pair(x, y + 1)) continue;
        if (y - 1 >= 0 && maze[x*COLS + y - 1] == 1 && previous[x*COLS + y] != make_pair(x, y - 1)) continue;
        maze[x*COLS + y] = 1;
        vector<pair<int, int> > to_stack;
        if (x + 1 < ROWS && !seen[(x + 1)*COLS + y]) {
            seen[(x + 1)*COLS + y] = true;
            to_stack.push_back(make_pair(x + 1, y));
            previous[(x + 1)*COLS + y] = make_pair(x, y);
        }
        if (x - 1 >= P0.first && !seen[(x - 1)*COLS + y]) {
            seen[(x - 1)*COLS + y] = true;
            to_stack.push_back(make_pair(x - 1, y));
            previous[(x - 1)*COLS + y] = make_pair(x, y);
        }
        if (y + 1 < COLS && !seen[x*COLS + y + 1]) {
            seen[x*COLS + y + 1] = true;
            to_stack.push_back(make_pair(x, y + 1));
            previous[x*COLS + y + 1] = make_pair(x, y);
        }
        if (y - 1 >= 0 && !seen[x*COLS + y - 1]) {
            seen[x*COLS + y - 1] = true;
            to_stack.push_back(make_pair(x, y - 1));
            previous[x*COLS + y - 1] = make_pair(x, y);
        }
        random_shuffle(to_stack.begin(), to_stack.end());
        bool pf_flag = false;
        for (pair<int,int>& neighbour : to_stack) {
            if (neighbour == Pf) pf_flag = true;
            else S.push(Cell(neighbour.first, neighbour.second));
        }
        if (pf_flag) S.push(Cell(Pf.first, Pf.second));
    }
    if(seen[Pf.first*COLS + Pf.second]==false){
        return  random_maze_generator(r,c,P0,Pf,start_val,end_val,maze);
    }
    int x0 = P0.first, y0 = P0.second;
    int xf = Pf.first, yf = Pf.second;
    maze[x0*COLS + y0] = start_val;
    maze[xf*COLS + yf] = end_val;
}

int getRandomInt(int N) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, N-1);
    return distr(gen);
}

void mazegenerator(int type){
    if(type == 0){
        mazegenerator_bfs(64,64);
    }
    else{
        mazegenerator_kruskal(64,64);
    }
}