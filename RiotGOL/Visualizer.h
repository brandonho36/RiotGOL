#include "Grid.h"
#include <SDL.h>

class Visualizer
{
public:
	Visualizer(int scale, int width, int height);
	bool drawGrid(Grid* grid);
	bool visualizePopulation(int startPopulation, int nextPopulation, int generation);

	int mPopulationScale;

private:

	int SCREEN_WIDTH = 1000;
	int SCREEN_HEIGHT = 1000;
	int SCALE;
	int MAX_GRAPH_POP;

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;

	SDL_Event e;
	};
