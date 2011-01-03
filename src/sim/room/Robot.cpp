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
	roomplannerptr.reset(new RoomPlanner(sensorpred, map, roomplannerconfig));

	curpos = pos;
	curdir = dir;
	map.clear(WorldGrid::UNKNOWN);

    const CoordScale &gridscale = roomplannerptr->getGridScale();
	Pos minpos = gridscale.coordToPos(pos.x-5, pos.y-5);
	Pos maxpos = gridscale.coordToPos(pos.x+5, pos.y+5);

	for (int x=minpos.x; x<=maxpos.x; x++) {
		for (int y=minpos.y; y<=maxpos.y; y++) {
			Pos p(x, y);
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

	if (abs(curdir - facedirrad) > .1)
		curdir = facedirrad;
	else if (plan.coords.size() > 1)
		curpos = plan.coords[1];
	else if (plan.identifyvictim)
		roomplannerptr->setVictimIdentified(plan.victimpos);
	else
		throw runtime_error("Do nothing route?");
}

void Robot::updateSensorsStep() {
    const CoordScale &gridscale = roomplannerptr->getGridScale();
	PosSet seenset = sensorpred.predictVision(curpos, curdir, grid, gridscale);

	for (PosSet::const_iterator i = seenset.begin(); i != seenset.end(); ++i) {
		map[*i] = grid[*i];
	}
}

void Robot::updatePathStep() {
	plan = roomplannerptr->planRoute(curpos, radToNearestDir(curdir));
}

