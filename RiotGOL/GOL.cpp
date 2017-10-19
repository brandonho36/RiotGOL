#include <iostream>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <sstream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include "Visualizer.h"
#include "Rules.h"
#include "Node.h"

using namespace std;
using namespace std::chrono;
using namespace std::this_thread;

const int DELAY_MS = 1;

const bool QUAD_TREE = true;
const bool RANDOMIZE = true;
const bool PRINT_TO_CONSOLE = true;

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 100;
const int SCALE = 1;

// How tall to make the population graph with respect to the starting population
const int POPULATION_SCALE = 1;

// How many coordinates to generate, and within what range
const int NUM_RAND_COORDS = 1000;
const int MAX_RAND_COORD = 40;


Grid* createInitialList() {
	std::vector<Coordinate*> aliveList;
	/*
	for (int i = 0; i < 20000; i++) {
		Coordinate* input = new Coordinate();
		input->x = rand() % SCREEN_WIDTH/SCALE/2  + SCREEN_WIDTH / SCALE / 4;
		input->y = rand() % SCREEN_HEIGHT/SCALE/2 + SCREEN_WIDTH / SCALE / 4;
		aliveList.push_back(input);
	}
	*/
	string line;
	ifstream myfile("input.txt");
	if (myfile.is_open()) {
		while (std::getline(myfile, line)) {
			Coordinate* input = new Coordinate();
			stringstream(line.substr(1, line.find(","))) >> input->x;
			stringstream(line.substr(line.find(",") + 1, line.length() - 1)) >> input->y;
			aliveList.push_back(input);
		}
	} 
	myfile.close();
	Grid* grid = new Grid();
	grid->aliveList = aliveList;
	return grid;

}

void print(Node* root) {
	/*
	std::cout << "Node start";
	std::cout << " ";
	std::cout << root->mLevel;
	std::cout << " ";
	std::cout << NodeFactory::cachedNodes.size();
	std::cout << " ";
	std::cout << root->mPopulation;
	std::cout << " "; */
	if (root->mNw->mLevel >= root->mLevel ||
		root->mSe->mLevel >= root->mLevel ||
		root->mSw->mLevel >= root->mLevel ||
		root->mNe->mLevel >= root->mLevel) {
		std::cout << "AAAAAAAAAAAAAAAABCBCCBCBCAAAA";
		std::cout << " ";
		std::cout << (root->mNw->mLevel);
		std::cout << " ";
		std::cout << (root->mNe->mLevel);
		std::cout << " ";
		std::cout << (root->mSw->mLevel);
		std::cout << " ";
		std::cout << (root->mSe->mLevel);
		std::cout << endl;
	}
	//std::cout << endl;
	if (root->mLevel == 62) {
		return;
	}
	/*

	//if (root->mPopulation != 0) {
	std::cout << "Level: ";
	std::cout << root->mLevel;
	std::cout << ", mPopulation";
	std::cout << root->mPopulation;
	std::cout << endl;
	//}
	if (root->mLevel == 60) {
	return;
	} */
	print(root->mNw);
	print(root->mNe);
	print(root->mSw);
	print(root->mSe);
}

unsigned long long generateLongLong(){
	unsigned long long r = 0;
	for (int i = 0; i<64; i++) {
		r = r * 2 + rand() % 2;
	}
	return r;
}

Node* insertCoordinates(Node* node) {
	string line;
	ifstream myfile("input.txt");
	Node* returnNode = node;
	if (!RANDOMIZE) {
		if (myfile.is_open()) {
			while (std::getline(myfile, line)) {
				long long x;
				long long y;
				stringstream(line.substr(1, line.find(","))) >> x;
				stringstream(line.substr(line.find(",") + 1, line.length() - 1)) >> y;
				unsigned long long newX = x;
				unsigned long long newY = y;
				if (x < 0) {
					newX -= _I64_MAX;
					newX--;
				}
				else {
					newX += _I64_MAX;
					newX++;
				}
				if (y < 0) {
					newY -= _I64_MAX;
					newY--;
				}
				else {
					newY += _I64_MAX;
					newY++;
				}/*
				cout << x;
				cout << ",";
				cout << y;
				cout << " ";
				cout << newX;
				cout << ",";
				cout << newY;
				cout << " "; */
				returnNode = returnNode->insert(newX, newY, _I64_MAX + 1ULL);
			}
		}
		myfile.close();
	}
	else {
		for (int i = 0; i < NUM_RAND_COORDS; i++) {
			unsigned long long x = rand() % MAX_RAND_COORD;
			unsigned long long y = rand() % MAX_RAND_COORD;
			cout << i;
			cout << " ";
			cout << x;
			cout << " ";
			cout << y;
			cout << endl;
			returnNode = returnNode->insert(x, y, _I64_MAX + 1ULL);
		}
	}
	return returnNode;
}
int main(int argc, char *args[]) {
	Visualizer visualizer(SCALE, SCREEN_WIDTH, SCREEN_HEIGHT);
	visualizer.mPopulationScale = POPULATION_SCALE;
	if (!QUAD_TREE) {

		Grid* grid = createInitialList();

		Rules rules;
		//While application is running
		while (!visualizer.drawGrid(grid))
		{
			rules.createNext(grid->aliveList);
			//sleep_for(50ms);
		}
	}
	else {
		Node* root = NodeFactory::createEmptyNode(64);
		root = insertCoordinates(root);
		std::cout << root->mPopulation;
		std::cout << endl;
		int i = 0;
		bool quit = false;
		int prevPopulation;
		while (!quit) {
			prevPopulation = root->mPopulation;
			if (PRINT_TO_CONSOLE) {
				std::cout << "Generation: ";
				std::cout << i;
				std::cout << ", Population: ";
				std::cout << root->mPopulation;
				std::cout << ", Cache size: ";
				std::cout << NodeFactory::cachedNodes.size();
				//std::cout << ", Level of root: ";
				//std::cout << root->mLevel;
				std::cout << endl;
			}
			root = root->expand();
			root = root->getNext();
			i++;
			quit = visualizer.visualizePopulation(prevPopulation, root->mPopulation, i);
		}
	}
	return 0;
}
