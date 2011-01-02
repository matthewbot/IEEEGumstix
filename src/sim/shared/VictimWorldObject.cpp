#include "ieee/sim/shared/VictimWorldObject.h"

using namespace ieee;

VictimWorldObject::VictimWorldObject(const Coord &coord) : coord(coord) { }
VictimWorldObject::~VictimWorldObject() { }

inline int sqr(int x) { return x*x; }

void VictimWorldObject::fillWorldGrid(WorldGrid &grid, const CoordScale &gridscale) const {
	Pos minpos = gridscale.coordToPos(coord.x - RADIUS, coord.y - RADIUS);
	Pos maxpos = gridscale.coordToPos(coord.x + RADIUS, coord.y + RADIUS);
	Pos centerpos(minpos.x + (maxpos.x - minpos.x)/2, minpos.y + (maxpos.y - minpos.y)/2);

	// TODO: crummy circle fill, fails if gridscale isn't square (although it should always be)
	for (int x = minpos.x; x <= maxpos.x; x++) {
		for (int y = minpos.y; y <= maxpos.y; y++) {
			Pos pos(x,y);

			if (!grid.inBounds(pos))
				continue;

			if (distance(pos, centerpos) > RADIUS / gridscale.sx)
				continue;

			grid[pos] = WorldGrid::VICTIM;
		}
	}
}

int VictimWorldObject::selectionTest(const Coord &selectcoord) const {
	if (distance(coord, selectcoord) <= RADIUS)
		return 1;
	else
		return -1;
}

void VictimWorldObject::selectionMoved(int id, const Coord &newcoord) {
	if (id == 1)
		coord = newcoord;
}

