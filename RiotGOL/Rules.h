#include "Grid.h"
#include <SDL.h>

class Rules
{
public:
	void createNext(std::vector<Coordinate*> &aliveList);

private:
	bool isValidCoordinate(long long x, long long y, int i, int j);



};
