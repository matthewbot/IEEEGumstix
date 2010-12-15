#include "ieee/sim/ObstacleWorldObject.h"
#include <algorithm>

using namespace ieee;
using namespace std;

ObstacleWorldObject::ObstacleWorldObject(const Pos &startpos, const Pos &endpos, bool large)
: startpos(startpos), endpos(endpos), large(large) { }
ObstacleWorldObject::~ObstacleWorldObject() { }

void ObstacleWorldObject::fillWorldGrid(WorldGrid &grid) const {
	fillLine(grid, startpos, endpos, large ? WorldGrid::LARGE_OBSTACLE : WorldGrid::SMALL_OBSTACLE);
}

int ObstacleWorldObject::selectionTest(const Pos &selectpos) const {
	if (selectpos == startpos)
		return 1;
	else if (selectpos == endpos)
		return 2;
	else
		return -1;
}

void ObstacleWorldObject::selectionMoved(int id, const Pos &newpos) {
	if (id == 1)
		startpos = newpos;
	else if (id == 2)
		endpos = newpos;
}


// based on bresenham's line algorithm
void ObstacleWorldObject::fillLine(WorldGrid &grid, Pos start, Pos end, WorldGrid::GridSquare square) {
	const bool steep = abs(end.y - start.y) > abs(end.x - start.x);
	if (steep) {
		swap(start.x, start.y);
		swap(end.x, end.y);
	}
	if (start.x > end.x) {
		swap(start.x, end.x);
		swap(start.y, end.y);
	}

	const float derror = (float)abs(end.y - start.y) / (end.x - start.x);

	float error = 0;
	int y = start.y;
	for (int x=start.x; x<=end.x; x++) {
		if (steep)
			grid[Pos(y, x)] = square;
		else
			grid[Pos(x, y)] = square;

		error += derror;
		if (error >= 0.5) {
			if (x+1 <= end.x) {
				if (steep) // small tweak, make sure there are no diagonal holes for it to pathfind through
					grid[Pos(y, x+1)] = square;
				else
					grid[Pos(x+1, y)] = square;
			}

			y += (start.y < end.y ? 1 : -1);

			if (x+1 <= end.x) {
				if (steep)
					grid[Pos(y, x)] = square;
				else
					grid[Pos(x, y)] = square;
			}

			error -= 1;
		}
	}
}

