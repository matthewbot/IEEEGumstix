#include "ieee/sim/room/Robot.h"
#include <algorithm>
#include <cmath>

using namespace ieee;
using namespace std;

Robot::Robot(const Coord &startpos, const WorldGrid &grid, const RoomPlanner::Config &roomplannerconfig)
: sensorpred(55, M_PI/4, .15),
  grid(grid),
  map(grid.getWidth(), grid.getHeight()),
  roomplanner(sensorpred, map, roomplannerconfig) {
	reset(startpos);
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

