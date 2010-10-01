#include "ObstacleWorldObject.h"

using namespace pathsim;

ObstacleWorldObject::ObstacleWorldObject(const Pos &startpos, const Pos &endpos, bool large)
: startpos(startpos), endpos(endpos), large(large) { }
ObstacleWorldObject::~ObstacleWorldObject() { }

void ObstacleWorldObject::fillWorldGrid(WorldGrid &grid) const {
	grid.fillLine(startpos, endpos, large ? WorldGrid::LARGE_OBSTACLE : WorldGrid::SMALL_OBSTACLE);
}

