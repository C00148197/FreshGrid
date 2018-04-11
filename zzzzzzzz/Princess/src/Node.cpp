#include "Node.h"

Node::Node(Vector pos, int id) {
	m_pos = pos;
	m_id = id;
}

void Node::addArc(Node *n) {
	//Arc arc;

	//arc.setNode(n);
	//arc.setWeight(calculateArcWeight(n->getPos()));



	//int temp = m_id;

	//int q = 5;

	//if (m_id == 2188)
	//{
	//	std::cout << "what " << std::endl;
	//}


	//m_arcs.push_back(arc);
}

float Node::calculateArcWeight(Vector otherNodePos) {


	return 1;//sqrt(((otherNodePos.x - m_pos.x) * (otherNodePos.x - m_pos.x)) + ((otherNodePos.y - m_pos.y) * (otherNodePos.y - m_pos.y)));
}

Vector Node::getPos() {
	return m_pos;
}

int Node::getID() {
	return m_id;
}

bool Node::getMarked() {
	return m_marked;
}

void Node::setMarked(bool marked) {
	m_marked = marked;
}

Node* Node::getPrevious() {
	return m_previous;
}

void Node::setPrevious(Node* previous) {
	m_previous = previous;
}

float Node::getHeuristic() {
	return m_heuristic;
}

void Node::setHeuristic(float heuristic) {
	m_heuristic = heuristic;
}

float Node::getCost() {
	return m_cost;
}

void Node::setCost(float cost) {
	m_cost = cost;
}

std::list<Arc>& Node::getArcs() {
	return m_arcs;
}