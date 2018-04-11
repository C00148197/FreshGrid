#include "Arc.h"

void Arc::setNode(Node *n) {
	m_node = n;
}

Node* Arc::getNode() {
	return m_node;
}

void Arc::setWeight(float w) {
	m_weight = w;
}

float Arc::getWeight() {
	return m_weight;
}