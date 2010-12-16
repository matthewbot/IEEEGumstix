#include "ieee/sim/Robot.h"
#include <algorithm>
#include <cmath>

using namespace ieee;
using namespace std;

Robot::Robot(const Coord &startpos, const WorldGrid &grid)
: sensorpred(55, M_PI/4, .15),
  grid(grid),
  map(grid.getWidth(), grid.getHeight()),
  roomplanner(sensorpred, map, roomplannerconfig) {
	reset(startpos);
}

Robot::RoomPlannerConfig::RoomPlannerConfig() {
    roomwidth = roomheight = 100;

    nodewidth = nodeheight = 10;
    nodeoffsetx = nodeoffsety = 0;

	victimwidth = victimheight = 10;
	victimoffsetx = victimoffsety = -.5;
	victimradius = 4;
	victimidentifyradius = 14;

	routeevalconfig.pathcostfactor = 1;
	routeevalconfig.revealedscorefactor = 6;
	routeevalconfig.turncostconstant = 4;
	routeevalconfig.turncostfactor = 1;
	routeevalconfig.turncostdivider = 2;
}

void Robot::reset(const Coord &pos, float dir) {
    const CoordScale &gridscale = roomplanner.getGridScale();
	curpos = pos;
	curdir = dir;
	map.clear(WorldGrid::UNKNOWN);

	Pos givenposes[] = {
		gridscale.coordToPos(pos.x-5, pos.y-5),
		gridscale.coordToPos(pos.x-5, pos.y+5),
		gridscale.coordToPos(pos.x+5, pos.y-5),
		gridscale.coordToPos(pos.x+5, pos.y+5),
	};

	for (int i=0; i<4; i++) {
		map[givenposes[i]] = grid[givenposes[i]];
	}

	roomplanner.resetVictims();

	updateSensorsStep();
	updatePathStep();
}

void Robot::step() {
	moveStep();
	updateSensorsStep();
	updatePathStep();
}

void Robot::moveStep() {
    if (plan.coords.size() == 0)
        return;

	float facedirrad = dirToRad(plan.facedirs[0]);

	if (abs(curdir - facedirrad) > .1)
		curdir = facedirrad;
	else if (plan.coords.size() > 1)
		curpos = plan.coords[1];
	else if (plan.identifyvictim)
		roomplanner.setVictimIdentified(plan.victimpos);
	else
		throw runtime_error("Do nothing route?");
}

void Robot::updateSensorsStep() {
    const CoordScale &gridscale = roomplanner.getGridScale();
	PosSet seenset = sensorpred.predictVision(curpos, curdir, grid, gridscale);

	for (PosSet::const_iterator i = seenset.begin(); i != seenset.end(); ++i) {
		map[*i] = grid[*i];
	}
}

void Robot::updatePathStep() {
	plan = roomplanner.planRoute(curpos, radToNearestDir(curdir));
}

