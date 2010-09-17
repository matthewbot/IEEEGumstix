#include "ObstacleWorldObject.h"

using namespace pathsim;

ObstacleWorldObject::ObstacleWorldObject(int startx, int starty, int endx, int endy, bool large)
: startx(startx), starty(starty), endx(endx), endy(endy), large(large) { }
ObstacleWorldObject::~ObstacleWorldObject() { }

void ObstacleWorldObject::fillWorldGrid(WorldGrid &grid) const {
	grid.fillLine(startx, starty, endx, endy, large ? WorldGrid::LARGE_OBSTACLE : WorldGrid::SMALL_OBSTACLE);
}

