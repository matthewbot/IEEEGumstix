#include "ieeepath/planner/TelepathicSensorPredictor.h"
#include <algorithm>

using namespace ieee;
using namespace std;

TelepathicSensorPredictor::TelepathicSensorPredictor(float range) : range(range) { }

PosSet TelepathicSensorPredictor::predictVision(const Coord &curpos, float curdir, const WorldGrid &grid) const {
	const int minx = max((int)(curpos.x-range), 0);
	const int maxx = min((int)(curpos.x+range), grid.getWidth()-1);
	const int miny = max((int)(curpos.y-range), 0);
	const int maxy = min((int)(curpos.y+range), grid.getHeight()-1);
	
	PosSet poses;
	
	for (int x=minx; x<=maxx; x++) {
		for (int y=miny; y<=maxy; y++) {
			poses.insert(Pos(x, y));
		}
	}
	
	return poses;
}

