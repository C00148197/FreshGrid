#include "AStar.h"

AStar::AStar(NodeLayout &nodes) : m_nodeLayout(nodes) {
	/********************************************//**
												  *  ...  constructor for astar
												  ***********************************************/
}

void AStar::calculatePath(Node* start, Node* dest, std::vector<Node*>& path) {
	/********************************************//**
												  *  ...  calculate astar path through nodes
												  ***********************************************/
	if (start != NULL) {
		// computes the actual cost of each path from dest to start
		ucs(dest, start, path);

		for (int i = 0; i < m_nodeLayout.getNoOfNodes() - 1; i++) {
			// calculates the heuristic of each node using the actual cost
			m_nodeLayout.getNodes()[i]->setHeuristic(m_nodeLayout.getNodes()[i]->getCost() * 0.9);

			// sets cost to infinity
			m_nodeLayout.getNodes()[i]->setCost(99999);
		}

		// sets up priority queue that compares the total cost and heuristic cost
		std::priority_queue<Node*, std::vector<Node*>, NodeSearchCostComparerAStar> priorityQueue;

		// set the cost to reach the start to be 0
		start->setCost(0);

		// push the starting node to the queue and set it as marked
		priorityQueue.push(start);
		start->setMarked(true);

		// if there are nodes still in the queue and the top of the queue is not the destination
		while (priorityQueue.size() != 0 && priorityQueue.top() != dest) {
			// add all of the child nodes of the parent node in the top of the queue, that have not been marked, into the queue
			// iterate through the list of arcs of the parent node
			auto iter = priorityQueue.top()->getArcs().begin();
			auto endIter = priorityQueue.top()->getArcs().end();

			for (; iter != endIter; iter++) {
				// adds the weight of the top of the queue to the weight of the the arc that it is looking at
				float distC = priorityQueue.top()->getCost() + (*iter).getWeight();

				// if this is less than the node weight of the node it is looking at
				if (distC < (*iter).getNode()->getCost()) {
					(*iter).getNode()->setCost(distC);
					// sets the top of the queue to be the previous node of this node
					(*iter).getNode()->setPrevious(priorityQueue.top());
				}

				if ((*iter).getNode()->getMarked() == false) {
					// mark the node and add it to the queue - puts priority on the shortest path
					priorityQueue.push((*iter).getNode());
					(*iter).getNode()->setMarked(true);
				}

				if ((*iter).getNode() == dest) {
					Node* temp = dest;

					if (distC <= (*iter).getNode()->getCost()) {
						if (path.empty() != true) {
							path.clear();
						}

						while (temp != start) {
							path.push_back(temp);
							temp = temp->getPrevious(); // goes back to the previous node until it reaches the initial node
						}
						path.push_back(start);
					}
				}
			}

			priorityQueue.pop(); // occasionally throws exception while in debug, but not in release (c++ STL re-heap issue)
		}
	}

	// resets nodes
	for (int i = 0; i < m_nodeLayout.getNoOfNodes() - 1; i++) {
		m_nodeLayout.getNodes()[i]->setMarked(false);
	}
}

void AStar::ucs(Node* start, Node* dest, std::vector<Node*>& path) {
	/********************************************//**
												  *  ...  usc implementation
												  ***********************************************/
	if (start != NULL) {
		for (int i = 0; i < m_nodeLayout.getNoOfNodes() - 1; i++) {
			// sets cost to infinity
			m_nodeLayout.getNodes()[i]->setCost(99999);
		}

		// sets up priority queue that compares the total cost and heuristic cost
		std::priority_queue<Node*, std::vector<Node*>, NodeSearchCostComparerUCS> priorityQueue;

		// set the cost to reach the start to be 0
		start->setCost(0);

		// push the starting node to the queue and set it as marked
		priorityQueue.push(start);
		start->setMarked(true);

		// if there are nodes still in the queue and the top of the queue is not the destination
		while (priorityQueue.size() != 0 && priorityQueue.top() != dest) {
			// add all of the child nodes of the parent node in the top of the queue, that have not been marked, into the queue
			// iterate through the list of arcs of the parent node
			auto iter = priorityQueue.top()->getArcs().begin();
			auto endIter = priorityQueue.top()->getArcs().end();

			for (; iter != endIter; iter++) {
				// adds the weight of the top of the queue to the weight of the the arc that it is looking at
				float distC = priorityQueue.top()->getCost() + (*iter).getWeight();

				// if this is less than the node weight of the node it is looking at
				if (distC < (*iter).getNode()->getCost()) {
					(*iter).getNode()->setCost(distC);

					// sets the top of the queue to be the previous node of this node
					(*iter).getNode()->setPrevious(priorityQueue.top());
				}

				if ((*iter).getNode()->getMarked() == false) {
					// mark the node and add it to the queue - puts priority on the shortest path
					priorityQueue.push((*iter).getNode());
					(*iter).getNode()->setMarked(true);
				}

				if ((*iter).getNode() == dest) {
					Node* temp = dest;

					if (distC <= (*iter).getNode()->getCost()) {
						if (path.empty() != true) {
							path.clear();
						}

						while (temp != start) {
							path.push_back(temp);
							temp = temp->getPrevious(); // goes back to the previous node until it reaches the initial node
						}
						path.push_back(start);
					}
				}
			}
			priorityQueue.pop();
		}
	}

	// resets nodes
	for (int i = 0; i < m_nodeLayout.getNoOfNodes() - 1; i++) {
		m_nodeLayout.getNodes()[i]->setMarked(false);
	}
}