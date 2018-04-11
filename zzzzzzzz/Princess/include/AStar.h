#pragma once

#include "NodeLayout.h"
#include "NodeCostComparer.h"
#include <queue>

class AStar {
public:
	AStar();
	AStar(NodeLayout &nodes);

	void calculatePath(Node* pStart, Node* pDest, std::vector<Node*>& path);

private:
	NodeLayout & m_nodeLayout;

	void ucs(Node* pStart, Node* pDest, std::vector<Node*>& path);
};