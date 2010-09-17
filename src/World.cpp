#include "World.h"
#include <boost/bind.hpp>
#include <algorithm>

using namespace pathsim;
using namespace boost;
using namespace std;

World::World(int w, int h) : grid(w, h) { }

World::iterator World::add(WorldObject *object) {
	iterator pos = objects.insert(objects.end(), object);
	object->fillWorldGrid(grid);
	return pos;
}

void World::remove(iterator object) {
	objects.erase(object);
	updateGrid();
}

void World::updateGrid() {
	grid.clear();
	for_each(objects.begin(), objects.end(), bind(&WorldObject::fillWorldGrid, _1, ref(grid)));
}

