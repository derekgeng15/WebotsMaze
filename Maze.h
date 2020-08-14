//Maze API
#pragma once
#include <cstdlib>
typedef unsigned char ubyte;
#define NUM_OF_DIRECTIONS 4
enum Direction {
	UP, RIGHT, DOWN, LEFT
};
struct Tile {
	ubyte U : 1;
	ubyte R : 1;
	ubyte D : 1;
	ubyte L : 1;
	ubyte victim : 1;
	ubyte seen : 3;
};
struct Maze {
	int width = 0;
	int length = 0;
	Tile* tiles = 0;
};
struct Wall {
	int t1 = 0;
	int t2 = 0;
};
Direction get_direction(Maze* m, int key);//returns direction from key(t1 - t2)
void gen_bMaze(Maze* m, int length, int width);//generates base maze
void set_wall(Maze* m, Wall* w);//sets wall in maze(blocks off path in corresponding tiles)
void block_tile(Maze* m, int t, Direction d);//block off a tile's path in a certain direction