#pragma once

#include "Node.h"

class NodeSearchCostComparerAStar {
public:
	bool operator()(Node* n1, Node* n2) {
		return n1->getCost() + n1->getHeuristic() > n2->getCost() + n2->getHeuristic();
	}
};

class NodeSearchCostComparerUCS {
public:
	bool operator()(Node* n1, Node* n2) {
		return n1->getCost() > n2->getCost();
	}
};