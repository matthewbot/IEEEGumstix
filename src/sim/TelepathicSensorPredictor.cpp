#include "TelepathicSensorPredictor.h"
#include <algorithm>

using namespace pathsim;
using namespace std;

TelepathicSensorPredictor::TelepathicSensorPredictor(int range) : range(range) { }

PosSet TelepathicSensorPredictor::predictVision(const Pos &curpos, const WorldGrid &grid) const {
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

