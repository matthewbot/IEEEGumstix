#include "ieee/drivers/laser/LaserPlot.h"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <sstream>

using namespace ieee;
using namespace boost;
using namespace boost::property_tree;
using namespace std;

LaserPlot::LaserPlot(const Config &config, const LaserSensor::Readings &readings, const Coord &curcoord, float curangle, WorldGrid &grid, const CoordScale &gridscale)
: squares(extents[grid.getWidth()][grid.getHeight()][config.maxlasers]) {
	fill(squares.data(), squares.data() + squares.num_elements(), 0);

	assert(readings.size() == config.maxlasers);

	for (int laser=0; laser<config.maxlasers; laser++) {
		const LaserSensor::DistAngleVec &distangles = readings[laser];

		for (LaserSensor::DistAngleVec::const_iterator i = distangles.begin(); i != distangles.end(); ++i) {
			Coord coord = i->toCoord(M_PI/2);

			Pos pos = gridscale.coordToPos(curcoord.x - coord.x, curcoord.y - coord.y);
			if (!grid.inBounds(pos))
				continue;

			squares[pos.x][pos.y][laser]++;
		}
	}

	for (int x=0; x<grid.getWidth(); x++) {
		for (int y=0; y<grid.getHeight(); y++) {
			for (int laser=0; laser<config.maxlasers; laser++) {
				if (squares[x][y][laser] >= config.minhits[laser]) {
					grid[Pos(x,y)] = config.squarelookup[laser];
					break;
				}
			}
		}
	}
}

void LaserPlot::Config::readTree(const ptree &pt) {
	maxlasers = pt.get<int>("maxlasers");
	maxangle = pt.get<float>("maxangle");

	minhits.resize(maxlasers);
	for (int i=0; i<maxlasers; i++) {
		stringstream buf;
		buf << "minhits_" << i;
		minhits[i] = pt.get<int>(buf.str());
	}

	squarelookup.resize(maxlasers);
	for (int i=0; i<maxlasers; i++) {
		stringstream buf;
		buf << "squarelookup_" << i;
		squarelookup[i] = (WorldGrid::GridSquare)pt.get<int>(buf.str());
	}
}

