#include "ieee/planner/CameraSensorPredictor.h"
#include <cmath>

using namespace ieee;
using namespace std;

CameraSensorPredictor::CameraSensorPredictor(float maxdistance, float fieldofview, float fovstep)
: maxdistance(maxdistance), fieldofview(fieldofview), fovstep(fovstep) { }

PosSet CameraSensorPredictor::predictVision(const Coord &curpos, float curdir, const WorldGrid &grid, const CoordScale &scale) const {
	const float startrad = curdir - fieldofview/2;
	const float endrad = startrad + fieldofview;

	PosSet seen;

	const float startx = curpos.x + .5f;
	const float starty = curpos.y + .5f;
	for (float rad=startrad; rad <= endrad; rad += fovstep) {
		const float dx = cos(rad);
		const float dy = -sin(rad);

		int deadspotheight = 0;
		float x = startx;
		float y = starty;
		for (int dist=0; dist<=maxdistance; dist++, x+=dx, y+=dy) {
			Pos pos = scale.coordToPos(x, y);

			if (!grid.inBounds(pos))
				break;

			int height=0;
			WorldGrid::GridSquare square = grid[pos];
			if (square == WorldGrid::LARGE_OBSTACLE)
				height = dist*3;
			else if (square == WorldGrid::VICTIM)
				height = dist;

			if (height < deadspotheight) {
				deadspotheight--;
				continue;
			}

			deadspotheight = height;
			seen.insert(pos);
		}
	}

	return seen;
}

