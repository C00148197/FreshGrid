#pragma once

#include "stdafx.h"
#include "Node.h"
#include "Arc.h"

#include <queue>

class NodeLayout {
public:
	NodeLayout() {}
	NodeLayout(std::vector<Vector> &nodeData);

	int getNoOfNodes();

	Node** getNodes();

	void setNodesPerLine(int value);

	void setSize(int value);

private:
	Node** m_nodes; // pointer to array of pointers to nodes

	int m_noOfNodes;

	void addArcs(); // adds connections between nodes

	int size = 20;

	int nodesPerLines = 20;
};
