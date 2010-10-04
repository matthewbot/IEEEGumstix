#include "ObstacleWorldObject.h"

using namespace pathsim;

ObstacleWorldObject::ObstacleWorldObject(const Pos &startpos, const Pos &endpos, bool large)
: startpos(startpos), endpos(endpos), large(large) { }
ObstacleWorldObject::~ObstacleWorldObject() { }

void ObstacleWorldObject::fillWorldGrid(WorldGrid &grid) const {
	grid.fillLine(startpos, endpos, large ? WorldGrid::LARGE_OBSTACLE : WorldGrid::SMALL_OBSTACLE);
}

int ObstacleWorldObject::selectionTest(const Pos &selectpos) const {
	if (selectpos == startpos)
		return 1;
	else if (selectpos == endpos)
		return 2;
	else
		return -1;
}

void ObstacleWorldObject::selectionMoved(int id, const Pos &newpos) {
	if (id == 1)
		startpos = newpos;
	else if (id == 2)
		endpos = newpos;
}

