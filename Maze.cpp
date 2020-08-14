#include "Maze.h"
Direction get_direction(Maze* m, int key) {
	int up = m->width;
	int down = -m->width;
	int left = 1;
	int right = -1;
	if (key == up)
		return UP;
	else if (key == down)
		return DOWN;
	else if (key == left)
		return LEFT;
	else if (key == right)
		return RIGHT;
}
void gen_bMaze(Maze* m, int length, int width) {
	m->width = width;
	m->length = length;
	//set dimensions
	m->tiles = (Tile*)malloc(m->length * m->width * sizeof(Tile));
	for (int i = 0; i < length; i++)
		for (int o = 0; o < width; o++) {
			m->tiles[i * width + o].U = 1;
			m->tiles[i * width + o].D = 1;
			m->tiles[i * width + o].L = 1;
			m->tiles[i * width + o].R = 1;
			m->tiles[i * width + o].seen = 0;
		}
	//set outer walls
	for (int i = 0; i < m->length; i++) {
		m->tiles[i * width].L = 0;
		m->tiles[i * width + m->width - 1].R = 0;
	}
	for (int i = 0; i < m->width; i++) {
		m->tiles[i].U = 0;
		m->tiles[(m->length - 1) * width + i].D = 0;
	}
}
void set_wall(Maze* m, Wall* w) {//in terms of t1
	if (get_direction(m, w->t1 - w->t2) == Direction::UP) {
		m->tiles[w->t1].U = 0;
		m->tiles[w->t2].D = 0;
	}
	else if (get_direction(m, w->t1 - w->t2) == Direction::DOWN) {
		m->tiles[w->t1].D = 0;
		m->tiles[w->t2].U = 0;
	}
	else if (get_direction(m, w->t1 - w->t2) == Direction::LEFT) {
		m->tiles[w->t1].L = 0;
		m->tiles[w->t2].R = 0;
	}
	else if (get_direction(m, w->t1 - w->t2) == Direction::RIGHT) {
		m->tiles[w->t1].R = 0;
		m->tiles[w->t2].L = 0;
	}
}
void block_tile(Maze* m, int t, Direction d) {
	switch (d) {
	case UP:
		m->tiles[t].U = 0;
		break;
	case RIGHT:
		m->tiles[t].R = 0;
		break;
	case DOWN:
		m->tiles[t].D = 0;
		break;
	case LEFT:
		m->tiles[t].L = 0;
		break;
	}
}