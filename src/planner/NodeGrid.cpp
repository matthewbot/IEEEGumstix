#include "ieeepath/planner/NodeGrid.h"

using namespace ieee;
using namespace boost;
using namespace std;

NodeGrid::NodeGrid(int width, int height) : array(extents[width][height]) { }

static Dir getVictimIDDir(const Pos &pos, const WorldGrid &grid);

NodeGrid NodeGrid::fromWorldGrid(const WorldGrid &grid) {
	NodeGrid nodes(grid.getWidth()-1, grid.getHeight()-1);
	
	for (int x=0; x<nodes.getWidth(); x++) {
		for (int y=0; y<nodes.getHeight(); y++) {
			Pos pos(x, y);
			Node &node = nodes[pos];
			
			// determine if passable
			if (!grid.passableRect(pos)) {
				node = Node::IMPASSABLE;
				continue;
			}
			
			if (grid.unknownRect(pos)) {
				node = Node::UNKNOWN;
				continue;
			}

			Dir victimdir = getVictimIDDir(pos, grid);
			if (victimdir != DIR_NONE) {
				node = Node(Node::VICTIM_ID, victimdir);
				continue;
			}
			
			nodes[pos] = Node::OPEN;
		}
	}
	
	return nodes;
}

std::ostream &ieee::operator<<(std::ostream &out, const NodeGrid &grid) {
	for (int y=0; y<grid.getHeight(); y++) {
		for (int x=0; x<grid.getWidth(); x++) {
			out << grid[Pos(x, y)];
		}
		
		out << endl;
	}
	
	return out;
}

