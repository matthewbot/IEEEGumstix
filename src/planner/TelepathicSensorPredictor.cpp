#include "ieeepath/planner/TelepathicSensorPredictor.h"
#include <algorithm>

using namespace ieee;
using namespace std;

TelepathicSensorPredictor::TelepathicSensorPredictor(float range) : range(range) { }

PosSet TelepathicSensorPredictor::predictVision(const Coord &curpos, float curdir, const WorldGrid &grid, const CoordScale &scale) const {
    Pos minpos = scale.coordToPos(Coord(curpos.x - range, curpos.y - range));
    Pos maxpos = scale.coordToPos(Coord(curpos.x + range, curpos.y + range));
	
	PosSet poses;
	for (int x=minpos.x; x<=maxpos.x; x++) {
		for (int y=minpos.y; y<=maxpos.y; y++) {
		    Pos p(x, y);
		    if (grid.inBounds(p))
			    poses.insert(p);
		}
	}
	
	return poses;
}

