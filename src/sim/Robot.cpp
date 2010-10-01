#include "Robot.h"
#include <algorithm>

using namespace pathsim;
using namespace std;

Robot::Robot(int sensorrange, const Pos &startpos, const WorldGrid &grid) 
: sensorrange(sensorrange), 
  grid(grid), 
  map(grid.getWidth(), grid.getHeight()),
  routeplanner(map, sensorrange) {
	reset(startpos);
}

void Robot::reset(const Pos &pos, Dir dir) {
	curpos = pos;
	curdir = dir;
	map.clear(WorldGrid::UNKNOWN);
	routeplanner.resetVictims();
	
	updateSensorsStep();
	updatePathStep();
}

void Robot::step() {
	moveStep();
	updateSensorsStep();
	updatePathStep();
}

void Robot::moveStep() {
	const Pos &newpos = route.path[0];
	curdir = getDirFromPoses(curpos, newpos);
	curpos = newpos;
	
	Pos victim;
	if (map.getAdjacent(curpos, WorldGrid::VICTIM, &victim))
		routeplanner.setVictimIdentified(victim);
}

void Robot::updateSensorsStep() {
	const int minx = max(curpos.x-sensorrange, 0);
	const int maxx = min(curpos.x+sensorrange, map.getWidth()-1);
	const int miny = max(curpos.y-sensorrange, 0);
	const int maxy = min(curpos.y+sensorrange, map.getHeight()-1);
	
	for (int x=minx; x<=maxx; x++) {
		for (int y=miny; y<=maxy; y++) {
			Pos pos(x, y);
			map[pos] = grid[pos];
		}
	}
}

void Robot::updatePathStep() {
	route = routeplanner.planRoute(curpos, curdir);
}

