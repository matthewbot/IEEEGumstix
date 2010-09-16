#include "RoomGrid.h"
#include <cassert>
#include <algorithm>

using namespace boost;
using namespace roomsim;
using namespace std;

RoomGrid::RoomGrid(int w, int h)
: width(w), height(h), squares(new GridSquare [w*h]) {
	fill(&squares[0], &squares[width*height], EMPTY);
}

RoomGrid::GridSquare &RoomGrid::operator()(int x, int y) {
	assert(x >= 0 && x < width);
	assert(y >= 0 && y < height);
	return squares[x + y*width];
}

RoomGrid::GridSquare RoomGrid::operator()(int x, int y) const {
	assert(x >= 0 && x < width);
	assert(y >= 0 && y < height);
	return squares[x + y*width];
}

bool RoomGrid::passable(GridSquare square) {
	switch (square) {
		case VICTIM:
		case LARGE_OBSTACLE:
			return false;
			
		default:
			return true;
	}
}

