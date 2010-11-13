#include "ieeepath/planner/Node.h"

using namespace ieee;
using namespace std;

Node::Node(Type type, Dir dir) : type(type), dir(dir) { }

bool Node::traverseableFrom(Dir travdir) const {
	if (type == OPEN || type == VICTIM_ID)
		return true;
	else if (type == IMPASSABLE || type == UNKNOWN)
		return false;
	else
		return dir == travdir || dir == oppositeDir(travdir);	
}

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

Dir ieee::getVictimIDDir(const Pos &nodepos, const WorldGrid &grid) {
	for (const victimcheck *check = victimchecks; check->dir != DIR_NONE; check++) {
		Pos pos(nodepos.x + check->dx, nodepos.y + check->dy);
		if (!grid.inBounds(pos))
			continue;
	
		if (grid[pos] == WorldGrid::VICTIM)
			return check->dir;
	}
	
	return DIR_NONE;
}

bool ieee::getVictimPos(const Pos &nodepos, const WorldGrid &grid, Dir dir, Pos &out) {
	for (const victimcheck *check = victimchecks; check->dir != DIR_NONE; check++) {
		if (check->dir != dir)
			continue;
			
		Pos pos(nodepos.x + check->dx, nodepos.y + check->dy);
		if (!grid.inBounds(pos))
			continue;
			
		if (grid[pos] == WorldGrid::VICTIM) {
			out = pos;
			return true;
		}
	}
	
	return false;
}

std::ostream &ieee::operator<<(std::ostream &out, const Node &node) {
	static const char typechars[] = " VCI-";
	out << typechars[node.getType()];
	return out;
}

