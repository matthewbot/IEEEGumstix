#include "World.h"
#include <boost/bind.hpp>
#include <algorithm>

using namespace pathsim;
using namespace boost;
using namespace std;

World::World(int w, int h) : grid(w, h) { }

World::WorldObjectIterator World::add(WorldObject *object) {
	objects.push_back(object);
	object->fillWorldGrid(grid);
}

void World::remove(WorldObjectIterator object) {
	objects.erase(object);
	updateGrid();
}

void World::updateGrid() {
	grid.clear();
	for_each(objects.begin(), objects.end(), bind(&WorldObject::fillWorldGrid, _1, ref(grid)));
}

