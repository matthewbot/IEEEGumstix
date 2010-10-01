#include "VictimWorldObject.h"

using namespace pathsim;

VictimWorldObject::VictimWorldObject(const Pos &pos) : pos(pos) { }
VictimWorldObject::~VictimWorldObject() { }

void VictimWorldObject::fillWorldGrid(WorldGrid &grid) const {
	grid[pos] = WorldGrid::VICTIM;
}

