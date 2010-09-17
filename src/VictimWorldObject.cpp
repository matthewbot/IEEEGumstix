#include "VictimWorldObject.h"

using namespace pathsim;

VictimWorldObject::VictimWorldObject(int x, int y) : x(x), y(y) { }
VictimWorldObject::~VictimWorldObject() { }

void VictimWorldObject::fillWorldGrid(WorldGrid &grid) const {
	grid(x, y) = WorldGrid::VICTIM;
}

