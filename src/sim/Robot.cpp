#include "Robot.h"
#include <algorithm>
#include <cmath>

using namespace pathsim;
using namespace std;

Robot::Robot(const Pos &startpos, const WorldGrid &grid) 
: sensorpred(5, M_PI/3, .05),
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
	
	Pos victim;
	if (map.getAdjacent(curpos, WorldGrid::VICTIM, &victim))
		routeplanner.setVictimIdentified(victim);
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

