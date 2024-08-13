#ifndef MAZESOLVER_HPP
#define MAZESOLVER_HPP
#include <iostream>
#include <string>
using namespace std;

extern int WALL;
extern int START;
extern int END;
extern int WAY;
extern int PATH;

string val(int n);
bool isValid(int x, int y, int rows, int cols);
void mazesolver(int type,string filename);

#endif