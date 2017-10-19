#include "Visualizer.h"
#include <SDL.h>
#include <cstdlib>
#include <iostream>

Visualizer::Visualizer(int scale, int width, int height) {

	SDL_Init(SDL_INIT_VIDEO);
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;
	mWindow = SDL_CreateWindow("Conway's Game of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
	SDL_RenderSetScale(mRenderer, scale, scale);
	SCALE = scale;
}

bool Visualizer::drawGrid(Grid* grid) {
	
	std::vector<Coordinate*> aliveList = grid->aliveList;

	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	SDL_RenderClear(mRenderer);

	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	 
	for (Coordinate* cell : aliveList) {

		SDL_Rect rect{ cell->x, cell->y, 1, 1 };
		SDL_RenderFillRect(mRenderer, &rect);
	}

	SDL_RenderPresent(mRenderer);

	bool quit = false;
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
		{
			quit = true;
			SDL_DestroyWindow(mWindow);
			SDL_Quit();
			break;
		}
	}
	return quit;
}

bool Visualizer::visualizePopulation(int startPopulation, int nextPopulation, int generation) {
	if (generation == 1) {
		SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
		SDL_RenderClear(mRenderer);
		MAX_GRAPH_POP = startPopulation * mPopulationScale;
	} /*
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	SDL_Rect rect{ 0, 0, startPopulation, startPopulation };
	SDL_RenderFillRect(mRenderer, &rect);
	*/
	startPopulation*MAX_GRAPH_POP*SCALE / SCREEN_HEIGHT;
		SDL_SetRenderDrawColor(mRenderer, 0,0,0, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawLine(mRenderer,
			generation - 1,
			SCREEN_HEIGHT - startPopulation*SCREEN_HEIGHT / MAX_GRAPH_POP,
			generation,
			SCREEN_HEIGHT - nextPopulation*SCREEN_HEIGHT / MAX_GRAPH_POP);
	SDL_RenderPresent(mRenderer);
	bool quit = false;
	while (SDL_PollEvent(&e) != 0)
	{
		//User requests quit
		if (e.type == SDL_QUIT)
		{
			quit = true;
			//Destroy window
			SDL_DestroyWindow(mWindow);

			//Quit SDL subsystems
			SDL_Quit();
			break;
		}
	}
	return quit;

}