#include "ieeepath/planner/NodeGrid.h"

using namespace ieeepath;
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
				node = Node::CLOSED;
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
}

static Dir getVictimIDDir(const Pos &pos, const WorldGrid &grid) {
	// determine if a victim check
	static const struct victimcheck {
		int dx, dy;
		Dir dir;
	} victimchecks[] = {
		{0, -1, DIR_N},
		{1, -1, DIR_N},
		{0, 2, DIR_S},
		{1, 2, DIR_S},
		{-1, 0, DIR_W},
		{-1, 1, DIR_W},
		{2, 0, DIR_E},
		{2, 1, DIR_E},
		{0, 0, DIR_NONE}
	};
	
	for (const victimcheck *check = victimchecks; check->dir != DIR_NONE; check++) {
		if (grid(pos.x + check->dx, pos.y + check->dy) == WorldGrid::VICTIM)
			return check->dir;
	}
	
	return DIR_NONE;
}


