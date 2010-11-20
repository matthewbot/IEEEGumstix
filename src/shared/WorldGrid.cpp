#include "ieeepath/shared/WorldGrid.h"
#include <cassert>
#include <algorithm>

using namespace boost;
using namespace ieee;
using namespace std;

WorldGrid::WorldGrid(int w, int h, GridSquare clearsquare)
: width(w), height(h), squares(new GridSquare [w*h]) {
	clear(clearsquare);
}

void WorldGrid::clear(GridSquare square) {
	fill(&squares[0], &squares[width*height], square);
}

