#include "Robot.h"
#include <algorithm>

using namespace pathsim;
using namespace std;

Robot::Robot(int sensorrange, const Pos &startpos, const WorldGrid &grid) 
: sensorpred(sensorrange), 
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
	const Pos &newpos = route.path[0];
	curdir = getDirFromPoses(curpos, newpos);
	curpos = newpos;
	
	Pos victim;
	if (map.getAdjacent(curpos, WorldGrid::VICTIM, &victim))
		routeplanner.setVictimIdentified(victim);
}

void Robot::updateSensorsStep() {
	PosSet seenset = sensorpred.predictVision(curpos, grid);
	
	for (PosSet::const_iterator i = seenset.begin(); i != seenset.end(); ++i) {
		map[*i] = grid[*i];
	}
}

void Robot::updatePathStep() {
	route = routeplanner.planRoute(curpos, curdir);
}

