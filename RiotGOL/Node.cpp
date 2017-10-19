#include "Node.h"
#include <stdio.h>
#include <iostream>
#include <cstdlib>

Node::Node(Node* nw, Node* ne, Node* sw, Node* se) {
	mNw = nw;
	mNe = ne;
	mSw = sw;
	mSe = se;
	mNext = nullptr;
	mLevel = nw->mLevel + 1;
	mIsAlive = false;
	// TODO: Consider integer overflows here
	mPopulation = mNw->mPopulation + mNe->mPopulation + mSw->mPopulation + mSe->mPopulation;
	/*
	stod::cout << mPopulation;
	std::cout << "=";
	std::cout << mNw->mPopulation;
	std::cout << "+";
	std::cout << mNe->mPopulation;
	std::cout << "+";
	std::cout << mSw->mPopulation;
	std::cout << "+";
	std::cout << mSe->mPopulation;
	std::cout << " "; */
	//std::cout << mLevel;
	//std::cout << std::endl;
	
}

Node::Node(bool isAlive) {
	mNw = nullptr;
	mNe = nullptr;
	mSw = nullptr;
	mSe = nullptr;
	mNext = nullptr;
	mLevel = 0;
	mIsAlive = isAlive;
	mPopulation = isAlive ? 1 : 0;
}

Node* Node::getVerticalMid(Node* n, Node* s) {
	return NodeFactory::createNode(n->mSw->mSe, n->mSe->mSw, s->mNw->mNe, s->mNe->mNw);
}

Node* Node::getHorizontalMid(Node* w, Node* e) {
	return NodeFactory::createNode(w->mNe->mSe, e->mNw->mSw, w->mSe->mNe, e->mSw->mNw);
}

Node* Node::getMid() {
	return NodeFactory::createNode(mNw->mSe, mNe->mSw, mSw->mNe, mSe->mNw);
}

Node* Node::getMidsMid() {
	return NodeFactory::createNode(mNw->mSe->mSe, mNe->mSw->mSw, mSw->mNe->mNe, mSe->mNw->mNw);
}

Node* Node::getSingleNode(Node* node, int aliveNeighbors) {
	if (node->mIsAlive) {
		return (aliveNeighbors == 2 || aliveNeighbors == 3) ? NodeFactory::aliveCell : NodeFactory::deadCell;
	}
	return aliveNeighbors == 3 ? NodeFactory::aliveCell : NodeFactory::deadCell;
}

Node* Node::getNextManually() {
	// TODO: Double check this, clean it up
	int aliveNeighbors[4] = { 0, 0, 0, 0 };

	if (mNw->mNw->mIsAlive) { aliveNeighbors[0]++; }
	if (mNw->mNe->mIsAlive) { aliveNeighbors[0]++; aliveNeighbors[1]++; }
	if (mNe->mNw->mIsAlive) { aliveNeighbors[0]++; aliveNeighbors[1]++; }
	if (mNe->mNe->mIsAlive) { aliveNeighbors[1]++; }

	if (mNw->mSw->mIsAlive) { aliveNeighbors[0]++; aliveNeighbors[2]++; }
	if (mNw->mSe->mIsAlive) { aliveNeighbors[1]++; aliveNeighbors[2]++; aliveNeighbors[3]++; }
	if (mNe->mSw->mIsAlive) { aliveNeighbors[0]++; aliveNeighbors[2]++; aliveNeighbors[3]++; }
	if (mNe->mSe->mIsAlive) { aliveNeighbors[1]++; aliveNeighbors[3]++; }

	if (mSw->mNw->mIsAlive) { aliveNeighbors[0]++; aliveNeighbors[2]++; }
	if (mSw->mNe->mIsAlive) { aliveNeighbors[0]++; aliveNeighbors[1]++; aliveNeighbors[3]++; }
	if (mSe->mNw->mIsAlive) { aliveNeighbors[0]++; aliveNeighbors[1]++; aliveNeighbors[2]++; }
	if (mSe->mNe->mIsAlive) { aliveNeighbors[1]++; aliveNeighbors[3]++; }

	if (mSw->mSw->mIsAlive) { aliveNeighbors[2]++; }
	if (mSw->mSe->mIsAlive) { aliveNeighbors[2]++; aliveNeighbors[3]++; }
	if (mSe->mSw->mIsAlive) { aliveNeighbors[2]++; aliveNeighbors[3]++; }
	if (mSe->mSe->mIsAlive) { aliveNeighbors[3]++; }

	Node* resultNw = getSingleNode(mNw->mSe, aliveNeighbors[0]);
	Node* resultNe = getSingleNode(mNe->mSw, aliveNeighbors[1]);
	Node* resultSw = getSingleNode(mSw->mNe, aliveNeighbors[2]);
	Node* resultSe = getSingleNode(mSe->mNw, aliveNeighbors[3]);

	return NodeFactory::createNode(resultNw, resultNe, resultSw, resultSe);
}

Node* Node::getNext() {
	/*
	if (mNw->mLevel >= mLevel ||
		mSe->mLevel >= mLevel ||
		mSw->mLevel >= mLevel ||
		mNe->mLevel >= mLevel) {
		std::cout << "AAAAAAAAAAAAAAAABCBCCBCBCAAAA";
		std::cout << " ";
		std::cout << (mNw->mLevel);
		std::cout << " ";
		std::cout << (mNe->mLevel);
		std::cout << " ";
		std::cout << (mSw->mLevel);
		std::cout << " ";
		std::cout << (mSe->mLevel);
		std::cout << std::endl;
	}
	*/
	if (mNext != nullptr) {
		return mNext;
	}
	if (mPopulation == 0) {
		mNext = mNw;
	}
	else if (mLevel == 2) {
		mNext = getNextManually();
	}
	else {
		Node* node00 = mNw->getMid();
		Node* node01 = getHorizontalMid(mNw, mNe);
		Node* node02 = mNe->getMid();
		Node* node10 = getVerticalMid(mNw, mSw);
		Node* node11 = getMidsMid();
		Node* node12 = getVerticalMid(mNe, mSe);
		Node* node20 = mSw->getMid();
		Node* node21 = getHorizontalMid(mSw, mSe);
		Node* node22 = mSe->getMid();
		Node* nextNw = NodeFactory::createNode(node00, node01, node10, node11)->getNext();
		Node* nextNe = NodeFactory::createNode(node01, node02, node11, node12)->getNext();
		Node* nextSw = NodeFactory::createNode(node10, node11, node20, node21)->getNext();
		Node* nextSe = NodeFactory::createNode(node11, node12, node21, node22)->getNext();
		mNext = NodeFactory::createNode(nextNw, nextNe, nextSw, nextSe);
	}
	return mNext;
}

Node* Node::expand() {
	if (mNw->mLevel >= mLevel ||
		mSe->mLevel >= mLevel ||
		mSw->mLevel >= mLevel ||
		mNe->mLevel >= mLevel) {
		std::cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBB";
		std::cout << std::endl;
	}
	Node* emptyBorder = NodeFactory::createEmptyNode(mLevel - 1);
	return NodeFactory::createNode(
		NodeFactory::createNode(emptyBorder, emptyBorder, emptyBorder, mNw),
		NodeFactory::createNode(emptyBorder, emptyBorder, mNe, emptyBorder),
		NodeFactory::createNode(emptyBorder, mSw, emptyBorder, emptyBorder),
		NodeFactory::createNode(mSe, emptyBorder, emptyBorder, emptyBorder));
}

// Consider the top left of all nodes as 0, 0. Insert into correct quadrant with new relative coordinates
Node* Node::insert(unsigned long long x, unsigned long long y, unsigned long long middle) {
	if (mLevel == 0) {
		return NodeFactory::aliveCell;
	}
	/*
	if (mNw->mLevel >= mLevel ||
		mSe->mLevel >= mLevel ||
		mSw->mLevel >= mLevel ||
		mNe->mLevel >= mLevel) {
		std::cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAAA";
		std::cout << std::endl;
	}
	std::cout << "mLevel: ";
	std::cout << mLevel;
	std::cout << " mMiddle Value ";
	std::cout << middle;
	std::cout << std::endl; 
	*/
	if (x < middle) {
		if (y < middle) {
			return NodeFactory::createNode(mNw->insert(x, y, middle / 2ULL), mNe, mSw, mSe);
		}
		else {
			return NodeFactory::createNode(mNw, mNe, mSw->insert(x, y - middle, middle / 2ULL), mSe);
		}
	}
	else {
		if (y < middle) {
			return NodeFactory::createNode(mNw, mNe->insert(x - middle, y, middle / 2ULL), mSw, mSe);
		}
		else {
			return NodeFactory::createNode(mNw, mNe, mSw, mSe->insert(x - middle, y - middle, middle / 2ULL));
		}
	}
}

std::unordered_map<Node, Node*> NodeFactory::cachedNodes;
Node* NodeFactory::aliveCell = new Node(true);
Node* NodeFactory::deadCell = new Node(false);