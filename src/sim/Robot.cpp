#include "ieeepath/sim/Robot.h"
#include <algorithm>
#include <cmath>

using namespace ieee;
using namespace std;

Robot::Robot(const Coord &startpos, const WorldGrid &grid) 
: sensorpred(7, M_PI/4, .15),
  grid(grid), 
  map(grid.getWidth(), grid.getHeight()),
  routeplanner(sensorpred, map, routeplannerconfig) {
	reset(startpos);
}

Robot::RouteEvaluatorConfig::RouteEvaluatorConfig() {
	unknownPruneDist = 2;
	pathCostFactor = 1;
	pathCostFactorVictim = 3;
	revealedScoreFactor = 6;
}

void Robot::reset(const Coord &pos, float dir) {
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
	float facedirrad = dirToRad(route.facedirs[0]);
	
	if (abs(curdir - facedirrad) > .1)
		curdir = facedirrad;
	else if (route.coords.size() > 1)
		curpos = route.coords[1];
	else if (route.identifyvictim)
		routeplanner.setVictimIdentified(route.victimpos);
	else
		throw runtime_error("Do nothing route?");
}

void Robot::updateSensorsStep() {
	PosSet seenset = sensorpred.predictVision(curpos, curdir, grid);
	
	for (PosSet::const_iterator i = seenset.begin(); i != seenset.end(); ++i) {
		map[*i] = grid[*i];
	}
}

void Robot::updatePathStep() {
	route = routeplanner.planRoute(Pos((int)floor(curpos.x), (int)(curpos.y)), radToNearestDir(curdir));
}

