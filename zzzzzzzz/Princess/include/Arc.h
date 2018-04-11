#pragma once

#include "stdafx.h"

class Node; // stops circular dependency between arc and node
class Arc {
public:
	Arc() {}

	//float getWeight();

	void setNode(Node *n);
	Node* getNode();

	void setWeight(float w);
	float getWeight();

private:
	Node * m_node;
	int m_weight;
};