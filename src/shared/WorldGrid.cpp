#include "ieee/shared/WorldGrid.h"
#include <cassert>
#include <algorithm>

using namespace boost;
using namespace ieee;
using namespace std;

WorldGrid::WorldGrid(int w, int h, GridSquare clearsquare)
: squares(extents[w][h]) {
	clear(clearsquare);
}

void WorldGrid::clear(GridSquare square) {
	fill(squares.data(), squares.data() + squares.num_elements(), square);
}

