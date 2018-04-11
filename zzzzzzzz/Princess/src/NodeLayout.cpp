#include "NodeLayout.h"

NodeLayout::NodeLayout(std::vector<Vector> &nodeData) {
	m_noOfNodes = nodeData.size() + 1;

	//size = 200;

	nodesPerLines = m_noOfNodes /20.0f;

	m_nodes = new Node*[m_noOfNodes];

	for (int i = 0; i < nodeData.size(); i++) {
		m_nodes[i] = new Node(nodeData.at(i), i);
		m_nodes[i]->setMarked(false);
	}

	auto temp = m_nodes[2188];



	addArcs();
}


void NodeLayout::setNodesPerLine(int incPerLine)
{
	nodesPerLines = incPerLine;
}

void NodeLayout::setSize(int incSize)
{
	size = incSize;
}

void NodeLayout::addArcs() {



//add this in loop maybe or something since they all need to know neighbours?
	int col = 1;

	for (int i = 0; i < m_noOfNodes; i++) {

		if (i == 2188)
		{
			std::cout << "whattttt" << std::endl;
		}

		if (i == (nodesPerLines * col)) {
			col++;
		}

		if ((i + nodesPerLines) < m_noOfNodes) {
			m_nodes[i]->addArc(m_nodes[i] + nodesPerLines);
			m_nodes[i+nodesPerLines]->addArc(m_nodes[i]);
		}

		if ((i + 1) < m_noOfNodes) {
			if (i < (nodesPerLines * col) - 1) {
				m_nodes[i]->addArc(m_nodes[i] + 1);
				m_nodes[i+1]->addArc(m_nodes[i]);
			}
		}

		if (i + 1 + nodesPerLines < m_noOfNodes) {
			if (i < (nodesPerLines * col) - 1) {
				m_nodes[i]->addArc(m_nodes[i] + 1 + nodesPerLines);
				m_nodes[i + 1 + nodesPerLines]->addArc(m_nodes[i]);
			}
		}

		if ((i + 1) - nodesPerLines >= 0) {
			m_nodes[i]->addArc(m_nodes[(i + 1) - nodesPerLines]);
			m_nodes[(i + 1) - nodesPerLines]->addArc(m_nodes[i]);
		}
	}

}

int NodeLayout::getNoOfNodes() {
	return m_noOfNodes;
}

Node** NodeLayout::getNodes() {
	return m_nodes;
}