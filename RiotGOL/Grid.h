#ifndef GRID_H
#define GRID_H
#include<vector>

struct Coordinate {
	long long x;
	long long y;
};

struct Grid {
	std::vector<Coordinate*> aliveList;
};
#endif GRID_H