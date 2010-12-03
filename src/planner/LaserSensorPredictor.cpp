#include "ieeepath/planner/LaserSensorPredictor.h"
#include <cmath>

using namespace ieee;
using namespace std;

LaserSensorPredictor::LaserSensorPredictor(float maxdistance, float fieldofview, float fovstep)
: maxdistance(maxdistance), fieldofview(fieldofview), fovstep(fovstep) { }

PosSet LaserSensorPredictor::predictVision(const Coord &curpos, float curdir, const WorldGrid &grid, const CoordScale &scale) const {
	const float startrad = curdir - fieldofview/2;
	const float endrad = startrad + fieldofview;

	PosSet seen;

	const float startx = curpos.x + .5f;
	const float starty = curpos.y + .5f;
	for (float rad=startrad; rad <= endrad; rad += fovstep) {
		const float dx = cos(rad);
		const float dy = -sin(rad);

		float x = startx;
		float y = starty;
		for (int dist=0; dist<=maxdistance; dist++, x+=dx, y+=dy) {
			Pos pos = scale.coordToPos(Coord(x, y));

			if (!grid.inBounds(pos))
				break;

			seen.insert(pos);

			if (grid[pos] == WorldGrid::LARGE_OBSTACLE)
				break;
		}
	}

	return seen;
}

