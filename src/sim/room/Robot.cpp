#include "ieee/sim/room/Robot.h"
#include <algorithm>
#include <cmath>

using namespace ieee;
using namespace std;

Robot::Robot(const WorldGrid &grid, const RoomPlanner::Config &roomplannerconfig, const Coord &pos, float dir)
: sensorpred(55, M_PI/4, .15),
  grid(grid),
  map(grid.getWidth(), grid.getHeight()) {
	reset(roomplannerconfig, pos, dir);
}

void Robot::reset(const RoomPlanner::Config &roomplannerconfig, const Coord &pos, float dir) {
	roomplannerconfigptr = &roomplannerconfig;
	roomplannerptr.reset(new RoomPlanner(sensorpred, map, roomplannerconfig));

	curpos = pos;
	curdir = dir;
	map.resizeClear(grid.getWidth(), grid.getHeight(), WorldGrid::UNKNOWN);

	Pos minpos = roomplannerconfig.gridscale.coordToPos(pos.x-10, pos.y-10);
	Pos maxpos = roomplannerconfig.gridscale.coordToPos(pos.x+10, pos.y+10);

	for (int x=minpos.x; x<=maxpos.x; x++) {
		for (int y=minpos.y; y<=maxpos.y; y++) {
			Pos p(x, y);
			if (grid.inBounds(p))
				map[p] = grid[p];
		}
	}

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

	if (abs(curdir - facedirrad) > .01)
		curdir = facedirrad;
	else if (plan.coords.size() > 1)
		curpos = plan.coords[1];
	else if (plan.identifyvictim)
		roomplannerptr->setVictimIdentified(plan.victimpos);
	else
		throw runtime_error("Do nothing route?");
}

void Robot::updateSensorsStep() {
	PosSet seenset = sensorpred.predictVision(curpos, curdir, grid, roomplannerconfigptr->gridscale);

	for (PosSet::const_iterator i = seenset.begin(); i != seenset.end(); ++i) {
		map[*i] = grid[*i];
	}
}

void Robot::updatePathStep() {
	plan = roomplannerptr->planRoute(curpos, radToNearestDir(curdir));
}

