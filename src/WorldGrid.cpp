#include "WorldGrid.h"
#include <cassert>
#include <algorithm>

using namespace boost;
using namespace pathsim;
using namespace std;

bool WorldGrid::passable(GridSquare square) {
	return square < VICTIM;
}

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

// based on bresenham's line algorithm
void WorldGrid::fillLine(int startx, int starty, int endx, int endy, GridSquare square) {
	const bool steep = abs(endy - starty) > abs(endx - startx);
	if (steep) {
		swap(startx, starty);
		swap(endx, endy);
	}
	if (startx > endx) {
		swap(startx, endx);
		swap(starty, endy);
	}
	
	const float derror = (float)abs(endy - starty) / (endx - startx);
	
	float error = 0;
	int y = starty;
	for (int x=startx; x<=endx; x++) {
		if (steep)
			set(y, x, square);
		else
			set(x, y, square);
			
		error += derror;
		if (error >= 0.5) {
			const bool horizdiag = error - 0.5 < derror/2;
			if (horizdiag) {
				if (steep) // small tweak, make sure there are no diagonal holes for it to pathfind through
					set(y, x+1, square);
				else
					set(x+1, y, square);
			}
				
			y += (starty < endy ? 1 : -1);
			
			if (!horizdiag) {
				if (steep)
					set(y, x, square);
				else
					set(x, y, square);
			}
			
			error -= 1;

		}
	}
}

