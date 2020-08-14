#include "MazeBFS.h"

void reset_parentArr(int* p, int size) {
	for (int i = 0; i < size; i++)
		p[i] = -1;
}
int newTile_BFS(Maze* m, int currTile, std::stack<int> * p, Direction d) {
	int* parent = new int[m->length * m->width];
	reset_parentArr(parent, m->length * m->width);
	parent[currTile] = currTile;
	std::queue<int> q;
	q.push(currTile);
	while (!q.empty()) {
		if (parent[q.front()] != q.front())
			d = get_direction(m, parent[q.front()] - q.front());
		if (m->tiles[q.front()].seen == 0) {
			currTile = q.front();
			break;
		}
		for (int i = 0; i < NUM_OF_DIRECTIONS; i++) {
			switch (d) {
			case UP:
				if (m->tiles[q.front()].U && parent[q.front() - m->width] == -1) {
					q.push(q.front() - m->width);
					parent[q.back()] = q.front();
				}
				break;
			case RIGHT:
				if (m->tiles[q.front()].R && parent[q.front() + 1] == -1) {
					q.push(q.front() + 1);
					parent[q.back()] = q.front();
				}
				break;
			case DOWN:
				if (m->tiles[q.front()].D && parent[q.front() + m->width] == -1) {
					q.push(q.front() + m->width);
					parent[q.back()] = q.front();
				}
				break;
			case LEFT:
				if (m->tiles[q.front()].L && parent[q.front() - 1] == -1) {
					q.push(q.front() - 1);
					parent[q.back()] = q.front();
				}
				break;

			}
			if (d == LEFT)
				 d = UP;
			else
				d = (Direction)(d + 1);
		}
		q.pop();
	}
	if (q.empty())
		return 0;
	int backtrack = currTile;
	while (backtrack != parent[backtrack]) {
		p->push(backtrack);
		backtrack = parent[backtrack];
	}

	delete parent;
	return 1;
}



int fTile_BFS(Maze* m, int currTile, std::stack<int>* p, Direction d, int targetTile) {
	int* parent = new int[m->length * m->width];
	reset_parentArr(parent, m->length * m->width);
	parent[currTile] = currTile;
	std::queue<int> q;
	q.push(currTile);
	while (!q.empty()) {
		if (parent[q.front()] != q.front())
			 d = get_direction(m, parent[q.front()] - q.front());
		if (q.front() == targetTile) {
			currTile = q.front();
			m->tiles[currTile].seen = 1;
			break;
		}
		for (int i = 0; i < NUM_OF_DIRECTIONS; i++) {
			switch (d) {
			case UP:
				if (m->tiles[q.front()].U && parent[q.front() - m->width] == -1) {
					q.push(q.front() - m->width);
					parent[q.back()] = q.front();
				}
				break;
			case RIGHT:
				if (m->tiles[q.front()].R && parent[q.front() + 1] == -1) {
					q.push(q.front() + 1);
					parent[q.back()] = q.front();
				}
				break;
			case DOWN:
				if (m->tiles[q.front()].D && parent[q.front() + m->width] == -1) {
					q.push(q.front() + m->width);
					parent[q.back()] = q.front();
				}
				break;
			case LEFT:
				if (m->tiles[q.front()].L && parent[q.front() - 1] == -1) {
					q.push(q.front() - 1);
					parent[q.back()] = q.front();
				}
				break;

			}
			if (d == LEFT)
				 d = UP;
			else
				d = (Direction)(d + 1);
		}
		q.pop();
	}
	if (q.empty())
		return 0;
	int backtrack = currTile;
	while (backtrack != parent[backtrack]) {
		p->push(backtrack);
		backtrack = parent[backtrack];
	}

	delete parent;
	return 1;
}