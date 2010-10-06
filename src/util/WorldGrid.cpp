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

bool WorldGrid::inBounds(const Pos &pos) const {
	if (pos.x < 0 || pos.x >= width)
		return false;
	if (pos.y < 0 || pos.y >= height)
		return false;
	return true;
}

WorldGrid::GridSquare &WorldGrid::operator[](const Pos &pos) {
	assert(inBounds(pos));
	return squares[pos.x + pos.y*width];
}

WorldGrid::GridSquare WorldGrid::operator[](const Pos &pos) const {
	assert(inBounds(pos));
	return squares[pos.x + pos.y*width];
}

// based on bresenham's line algorithm
void WorldGrid::fillLine(Pos start, Pos end, GridSquare square) {
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
			set(y, x, square);
		else
			set(x, y, square);
			
		error += derror;
		if (error >= 0.5) {
			const bool horizdiag = error - 0.5 <= derror/2;
			if (horizdiag && x+1 <= end.x) {
				if (steep) // small tweak, make sure there are no diagonal holes for it to pathfind through
					set(y, x+1, square);
				else
					set(x+1, y, square);
			}
			
			y += (start.y < end.y ? 1 : -1);
			
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

int WorldGrid::countAdjacent(const Pos &pos, GridSquare square, int range) const {
	int count=0;

	for (int dx=-range; dx<=range; dx++) {
		for (int dy=-range; dy<=range; dy++) {
			if (dx == 0 && dy == 0)
				continue;
				
			Pos checkpos(pos.x+dx, pos.y+dy);
			
			if (!inBounds(checkpos))
				continue;
				
			if (get(checkpos) == square)
				count++;
		}
	}
	
	return count;
}

bool WorldGrid::getAdjacent(const Pos &pos, GridSquare square, Pos *out, int range) const {
	for (int dx=-range; dx<=range; dx++) {
		for (int dy=-range; dy<=range; dy++) {
			if (dx == 0 && dy == 0)
				continue;
			
			Pos checkpos(pos.x+dx, pos.y+dy);
			
			if (!inBounds(checkpos))
				continue;
				
			if (get(checkpos) == square) {
				if (out) 
					*out = checkpos;
				
				return true;
			}
		}
	}
	
	return false;
}


