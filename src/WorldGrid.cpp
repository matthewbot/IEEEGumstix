#include "WorldGrid.h"
#include <cassert>
#include <algorithm>

using namespace boost;
using namespace roomsim;
using namespace std;

WorldGrid::WorldGrid(int w, int h)
: width(w), height(h), squares(new GridSquare [w*h]) {
	clear();
}

void WorldGrid::clear() {
	fill(&squares[0], &squares[width*height], EMPTY);
}

WorldGrid::GridSquare &WorldGrid::operator()(int x, int y) {
	assert(x >= 0 && x < width);
	assert(y >= 0 && y < height);
	return squares[x + y*width];
}

WorldGrid::GridSquare WorldGrid::operator()(int x, int y) const {
	assert(x >= 0 && x < width);
	assert(y >= 0 && y < height);
	return squares[x + y*width];
}

bool WorldGrid::passable(GridSquare square) {
	switch (square) {
		case VICTIM:
		case LARGE_OBSTACLE:
			return false;
			
		default:
			return true;
	}
}

