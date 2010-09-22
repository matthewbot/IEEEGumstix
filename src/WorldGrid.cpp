#include "WorldGrid.h"
#include <cassert>
#include <algorithm>

using namespace boost;
using namespace pathsim;
using namespace std;

WorldGrid::WorldGrid(int w, int h, GridSquare clearsquare)
: width(w), height(h), squares(new GridSquare [w*h]) {
	clear(clearsquare);
}

void WorldGrid::clear(GridSquare square) {
	fill(&squares[0], &squares[width*height], square);
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

int WorldGrid::countAdjacent(int x, int y, GridSquare square, int range) const {
	int count=0;

	for (int dx=-range; dx<=range; dx++) {
		for (int dy=-range; dy<=range; dy++) {
			if (dx == 0 && dy == 0)
				continue;
				
			const int checkx = x+dx;
			const int checky = y+dy;
			
			if (checkx < 0 || checkx >= width)
				continue;
			if (checky < 0 || checky >= height)
				continue;
				
			if (get(checkx, checky) == square)
				count++;
		}
	}
	
	return count;
}

bool WorldGrid::getAdjacent(int x, int y, GridSquare square, int *outx, int *outy, int range) const {
	for (int dx=-range; dx<=range; dx++) {
		for (int dy=-range; dy<=range; dy++) {
			if (dx == 0 && dy == 0)
				continue;
			
			const int checkx = x+dx;
			const int checky = y+dy;
			
			if (checkx < 0 || checkx >= width)
				continue;
			if (checky < 0 || checky >= height)
				continue;
				
			if (get(checkx, checky) == square) {
				if (outx && outy) {
					*outx = checkx;
					*outy = checky;
				}
				
				return true;
			}
		}
	}
	
	return false;
}


