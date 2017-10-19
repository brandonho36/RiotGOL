#ifndef NODE_H
#define NODE_H
#include<unordered_map>
#include <stdio.h>
#include <iostream>
#include <cstdlib>

class Node {
public:
	Node(Node* nw, Node* ne, Node* sw, Node* se);
	Node(bool isAlive);

	Node* mNw;
	Node* mNe;
	Node* mSw;
	Node* mSe;
	int mLevel;
	int mPopulation;
	bool mIsAlive;

	Node* getNext();
	Node* expand();
	Node* insert(unsigned long long x, unsigned long long y, unsigned long long middle);

private:
	Node* mNext;

	Node* getVerticalMid(Node* n, Node* s);
	Node* getHorizontalMid(Node* w, Node* e);
	Node* getMid();
	Node* getMidsMid();
	Node* getSingleNode(Node* node, int aliveNeighbors);
	Node* getNextManually();
};
inline
bool operator==(const Node& lhs, const Node&rhs) {
	if (lhs.mLevel != rhs.mLevel) {
		return false;
	}
	if (lhs.mLevel == 0) {
		return lhs.mIsAlive == rhs.mIsAlive;
	}
	return lhs.mNw == rhs.mNw && lhs.mNe == rhs.mNe && lhs.mSe == rhs.mSe && lhs.mSw == rhs.mSw;
}

namespace std {
	template <> struct hash<Node>
	{
		size_t operator()(const Node & node) const
		{
			if (node.mLevel == 0) {
				return node.mPopulation;
			}
			size_t nwPtr = reinterpret_cast<size_t>(node.mNw);
			size_t nePtr = reinterpret_cast<size_t>(node.mNe);
			size_t swPtr = reinterpret_cast<size_t>(node.mSw);
			size_t sePtr = reinterpret_cast<size_t>(node.mSe);
			return nwPtr * 11 + nePtr * 31 + swPtr * 107 + sePtr * 599;
		}
	};
}

class NodeFactory {
public:
	static std::unordered_map<Node, Node*> cachedNodes;
	static Node* aliveCell;
	static Node* deadCell;

	// Cache any nodes below this level. 
	static const int storageLevel = 60;

	static Node* createNode(Node* nw, Node* ne, Node* sw, Node* se) {
		//TODO: Implement caching levels here
		Node* newNode = new Node(nw, ne, sw, se);
		/*if (newNode->mLevel > storageLevel) {
			return newNode;
		}*/
		Node* cachedNode = NodeFactory::cachedNodes[*newNode];
		if (cachedNode == nullptr) {
			NodeFactory::cachedNodes[*newNode] = newNode;
			return newNode;
		}
		else {
			free(newNode);
			return cachedNode;
		}
	}
	static Node* createNode(bool isAlive) {
		return isAlive ? aliveCell : deadCell;
	}
	static Node* createEmptyNode(int level) {
		if (level == 0) {
			return deadCell;
		}
		else {
			Node* child = NodeFactory::createEmptyNode(level - 1);
			return createNode(child, child, child, child);
		}
	}
};

#endif