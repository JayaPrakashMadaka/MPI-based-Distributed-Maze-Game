#ifndef MAZEGENERATOR_HPP
#define MAZEGENERATOR_HPP
#include <stack>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono> 
#include <iostream>
using namespace std;

void mazegenerator(int type);
void random_maze_generator(int r, int c, pair<int, int> P0, pair<int, int> Pf,int start_val,int end_val,int* maze);
void random_maze_generator_kruskal(int r, int c, pair<int, int> P0, pair<int, int> Pf,int start_val,int end_val,int* maze);
int getRandomInt(int N);

#endif