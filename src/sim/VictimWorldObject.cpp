#include "ieeepath/sim/VictimWorldObject.h"

using namespace ieee;

VictimWorldObject::VictimWorldObject(const Pos &pos) : pos(pos) { }
VictimWorldObject::~VictimWorldObject() { }

void VictimWorldObject::fillWorldGrid(WorldGrid &grid) const {
	grid[pos] = WorldGrid::VICTIM;
}

int VictimWorldObject::selectionTest(const Pos &selectpos) const {
	if (pos == selectpos)
		return 1;
	else
		return -1;
}

void VictimWorldObject::selectionMoved(int id, const Pos &newpos) {
	if (id == 1)
		pos = newpos;
}
