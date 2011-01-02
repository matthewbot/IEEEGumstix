#include "ieee/sim/shared/ObstacleWorldObject.h"
#include <algorithm>

using namespace std;
using namespace ieee;

ObstacleWorldObject::ObstacleWorldObject(const Coord &startcoord, const Coord &endcoord, bool large)
: startcoord(startcoord), endcoord(endcoord), large(large) { }

ObstacleWorldObject::~ObstacleWorldObject() { }

void ObstacleWorldObject::fillWorldGrid(WorldGrid &grid, const CoordScale &gridscale) const {
	const float thickness = getWidth()/2;
	const float dir = atan2(endcoord.y-startcoord.y, endcoord.x - startcoord.x);

	Coord coords[4];
	coords[0].x = startcoord.x + thickness*cos(dir + 3*M_PI/4);
	coords[0].y = startcoord.y + thickness*sin(dir + 3*M_PI/4);
	coords[1].x = startcoord.x + thickness*cos(dir - 3*M_PI/4);
	coords[1].y = startcoord.y + thickness*sin(dir - 3*M_PI/4);
	coords[2].x = endcoord.x + thickness*cos(dir - M_PI/4);
	coords[2].y = endcoord.y + thickness*sin(dir - M_PI/4);
	coords[3].x = endcoord.x + thickness*cos(dir + M_PI/4);
	coords[3].y = endcoord.y + thickness*sin(dir + M_PI/4);

	Pos poses[4];
	for (int i=0; i<4; i++)
		poses[i] = gridscale.coordToPos(coords[i]);

	const WorldGrid::GridSquare square = large ? WorldGrid::LARGE_OBSTACLE : WorldGrid::SMALL_OBSTACLE;
	shadeQuad(poses, grid, square);
}

int ObstacleWorldObject::selectionTest(const Coord &selectcoord) const {
	float rad = getWidth()/2;

	if (distance(startcoord, selectcoord) <= rad)
		return 1;
	else if (distance(endcoord, selectcoord) <= rad)
		return 2;
	else
		return -1;
}

void ObstacleWorldObject::selectionMoved(int id, const Coord &newcoord) {
	if (id == 1)
		startcoord = newcoord;
	else if (id == 2)
		endcoord = newcoord;
}

float ObstacleWorldObject::getWidth() const {
	return large ? LARGE_WIDTH : SMALL_WIDTH;
}

const float ObstacleWorldObject::LARGE_WIDTH;
const float ObstacleWorldObject::SMALL_WIDTH;

void ObstacleWorldObject::shadeQuad(const Pos (&poses)[4], WorldGrid &grid, WorldGrid::GridSquare square) {
	pair<Pos, Pos> minmax = findMinMax<Pos>(&poses[0], &poses[4]);
	const Pos &min = minmax.first;
	const Pos &max = minmax.second;

	for (int x=min.x; x<=max.x; x++) {
		for (int y=min.y; y<=max.y; y++) {
			Pos pos(x, y);
			if (!grid.inBounds(pos))
				continue;

			bool inside;

			for (int i=0; i<4; i++) {
				const Pos &start = poses[i];
				const Pos &end = poses[(i+1) % 4];

				if (start.x != end.x) {
					const float m = (float)(end.y - start.y) / (end.x - start.x);
					float yvala = start.y + m*((x-.5f) - start.x);
					float yvalb = start.y + m*((x+.5f) - start.x);

					if (start.x > end.x)
						inside = y <= std::max(yvala, yvalb)+.5f;
					else
						inside = y >= std::min(yvala, yvalb)-.5f;
				} else if (start.y != end.y) {
					if (start.y < end.y)
						inside = x <= start.x;
					else
						inside = x >= start.x;
				} else
					inside = true;

				if (!inside)
					break;
			}

			if (inside)
				grid[pos] = square;
		}
	}
}

