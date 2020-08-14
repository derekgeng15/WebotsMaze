//Maze Breadth First Search Algorithm
#pragma once
#include "Maze.h"
#include <queue>
#include <stack>
#include <cstring>
#include <cstdlib>

void reset_parentArr(int* p, int size);
int newTile_BFS(Maze* m, int currTile, std::stack<int> * p, Direction d);//BFS for finding a new tile. Returns 0 if none found
int fTile_BFS(Maze* m, int currTile, std::stack<int> * p, Direction d, int targetTile);//BFS for finding specific tile. Returns 0 if not found
