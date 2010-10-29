#include "ieeepath/sim/Robot.h"
#include <algorithm>
#include <cmath>

using namespace ieeepath;
using namespace std;

Robot::Robot(const Pos &startpos, const WorldGrid &grid) 
: sensorpred(7, M_PI/4, .15),
  grid(grid), 
  map(grid.getWidth(), grid.getHeight()),
  routeplanner(sensorpred, map) {
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
	if (curdir != route.facedirs[0])
		curdir = getIntermediateDir(curdir, route.facedirs[0]);
	else if (route.path.size() > 1)
		curpos = route.path[1];
	
	Pos facepos = advancePos(curpos, curdir);
	if (grid.inBounds(facepos) && grid[facepos] == WorldGrid::VICTIM)
		routeplanner.setVictimIdentified(facepos);
}

void Robot::updateSensorsStep() {
	PosSet seenset = sensorpred.predictVision(curpos, curdir, grid);
	
	for (PosSet::const_iterator i = seenset.begin(); i != seenset.end(); ++i) {
		map[*i] = grid[*i];
	}
}

void Robot::updatePathStep() {
	route = routeplanner.planRoute(curpos, curdir);
}

