#include "ieeepath/planner/TelepathicSensorPredictor.h"
#include <algorithm>

using namespace ieeepath;
using namespace std;

TelepathicSensorPredictor::TelepathicSensorPredictor(int range) : range(range) { }

PosSet TelepathicSensorPredictor::predictVision(const Pos &curpos, Dir curdir, const WorldGrid &grid) const {
	const int minx = max(curpos.x-range, 0);
	const int maxx = min(curpos.x+range, grid.getWidth()-1);
	const int miny = max(curpos.y-range, 0);
	const int maxy = min(curpos.y+range, grid.getHeight()-1);
	
	PosSet poses;
	
	for (int x=minx; x<=maxx; x++) {
		for (int y=miny; y<=maxy; y++) {
			poses.insert(Pos(x, y));
		}
	}
	
	return poses;
}

