#include "Rules.h"
#include <unordered_map>

using namespace std;

bool Rules::isValidCoordinate(long long x, long long y, int i, int j) {
	// Check to make sure it's not overflow, and is not the given cell
	return !(i == 0 && j == 0) &&
		!(x == LLONG_MIN && i == -1) &&
		!(x == LLONG_MAX && i == 1) &&
		!(y == LLONG_MIN && j == -1) &&
		!(y == LLONG_MAX && j == 1);
}

void Rules::createNext(std::vector<Coordinate*> &aliveList) {
	std::unordered_map<long long, std::unordered_map<long long, int>> neighborsMap;
	for (Coordinate* cell : aliveList) {
		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; j++) {
				if (isValidCoordinate(cell->x, cell->y, i, j)) {
					neighborsMap[cell->x + i][cell->y + j]++;
				}
			}
		}
	}
	for (int i = aliveList.size() - 1; i >= 0; i--) {
		Coordinate* cell = aliveList.at(i);
		int numAliveNeighbors = neighborsMap[cell->x][cell->y];
		neighborsMap[cell->x].erase(cell->y);
		if (neighborsMap[cell->x].size() == 0) {
			neighborsMap.erase(cell->x);
		}
		if (numAliveNeighbors < 2 || numAliveNeighbors > 3) {
			aliveList.erase(aliveList.begin() + i);
			delete(cell);
		}
	}
	for (pair<long long, std::unordered_map<long long, int>> kvPair : neighborsMap) {
		for (pair<long long, int> kvPair2 : kvPair.second) {
			if (kvPair2.second == 3) {
				Coordinate* aliveCell = new Coordinate();
				aliveCell->x = kvPair.first;
				aliveCell->y = kvPair2.first;
				aliveList.push_back(aliveCell);
			}
		}
	}
}