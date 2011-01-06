#include "ieee/planner/NodeGrid.h"

using namespace ieee;
using namespace boost;
using namespace std;

NodeGrid::NodeGrid() { }
NodeGrid::NodeGrid(int width, int height) : array(extents[width][height]) { }
NodeGrid::NodeGrid(const NodeGrid &nodegrid) : array(nodegrid.array) { }

NodeGrid &NodeGrid::operator=(const NodeGrid &nodegrid) {
	array.resize(extents[nodegrid.getWidth()][nodegrid.getHeight()]);
	array = nodegrid.array;
}

static Dir getVictimIDDir(const Pos &pos, const WorldGrid &grid);

NodeGrid NodeGrid::fromWorldGrid(const WorldGrid &grid, const CoordScale &gridscale, const CoordScale &nodescale) {
    Pos gridendpos(grid.getWidth(), grid.getHeight());
    Pos nodeendpos = nodescale.coordToPos(gridscale.posToCoord(gridendpos));

	NodeGrid nodes(nodeendpos.x, nodeendpos.y);

	for (int x=0; x<nodes.getWidth(); x++) {
		for (int y=0; y<nodes.getHeight(); y++) {
			Pos pos(x, y);
			Coord coord = nodescale.posToCoord(pos);
			Node &node = nodes[pos];

			Pos mingridpos = gridscale.coordToPos(coord.x-5, coord.y-5);
			Pos maxgridpos = gridscale.coordToPos(coord.x+5, coord.y+5);

			// determine if passable
			if (!passableRect(grid, mingridpos, maxgridpos)) {
				node = Node::IMPASSABLE;
				continue;
			}

			if (unknownRect(grid, mingridpos, maxgridpos)) {
				node = Node::UNKNOWN;
				continue;
			}

			nodes[pos] = Node::OPEN;
		}
	}

	return nodes;
}

bool NodeGrid::passableRect(const WorldGrid &grid, const Pos &start, const Pos &end) {
	for (int x=start.x; x<=end.x; x++) {
		for (int y=start.y; y<=end.y; y++) {
			Pos pos(x, y);

			if (!grid.inBounds(pos))
			    return false;

			if (!passable(grid[pos]))
				return false;
		}
	}

	return true;
}

bool NodeGrid::unknownRect(const WorldGrid &grid, const Pos &start, const Pos &end) {
	for (int x=start.x; x<=end.x; x++) {
		for (int y=start.y; y<=end.y; y++) {
			Pos pos(x, y);

			if (!grid.inBounds(pos))
			    continue;

			if (grid[pos] == WorldGrid::UNKNOWN)
				return true;
		}
	}

	return false;
}

bool NodeGrid::passable(WorldGrid::GridSquare square) {
	return square != WorldGrid::LARGE_OBSTACLE && square != WorldGrid::VICTIM;
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

