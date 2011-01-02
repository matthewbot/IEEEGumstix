#include "ieee/sim/shared/World.h"
#include <boost/bind.hpp>
#include <algorithm>

using namespace ieee;
using namespace boost;
using namespace std;

World::World(int w, int h) : grid(w, h), gridscale(w/100.0f, h/100.0f, -.5, -.5) { }

World::iterator World::add(WorldObject *object) {
	iterator pos = objects.insert(objects.end(), object);
	object->fillWorldGrid(grid, gridscale);
	return pos;
}

void World::remove(iterator object) {
	objects.erase(object);
	updateGrid();
}

void World::updateGrid() {
	grid.clear();
	for_each(objects.begin(), objects.end(), bind(&WorldObject::fillWorldGrid, _1, ref(grid), cref(gridscale)));
}

