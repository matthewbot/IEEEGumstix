#include "CameraSensorPredictor.h"
#include <cmath>

using namespace pathsim;
using namespace std;

CameraSensorPredictor::CameraSensorPredictor(int maxdistance, float fieldofview, float fovstep) 
: maxdistance(maxdistance), fieldofview(fieldofview), fovstep(fovstep) { }

PosSet CameraSensorPredictor::predictVision(const Pos &curpos, Dir curdir, const WorldGrid &grid) const {
	const float startrad = dirToRad(curdir) - fieldofview/2;
	const float endrad = startrad + fieldofview;
	
	PosSet seen;
	
	for (float rad=startrad; rad <= endrad; rad += fovstep) {
		int deadspotheight = 0;
	
		for (int dist=0; dist<=maxdistance; dist++) {
			float x = curpos.x + dist*cos(rad);
			float y = curpos.y - dist*sin(rad); // our y axis is inverted
			Pos pos((int)floor(x), (int)floor(y));
			
			if (!grid.inBounds(pos))
				break;
			
			int height=0;
			WorldGrid::GridSquare square = grid[pos];
			if (square == WorldGrid::LARGE_OBSTACLE)
				height = dist*3;
			else if (square == WorldGrid::VICTIM)
				height = dist/2;
				
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

