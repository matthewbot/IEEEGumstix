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
: config(config),
  laserhits(extents[grid.getWidth()][grid.getHeight()][config.maxlasers]),
  emptyhits(extents[grid.getWidth()][grid.getHeight()]) {
  	assert(readings.size() == config.maxlasers);
	fill(laserhits.data(), laserhits.data() + laserhits.num_elements(), 0);
	fill(emptyhits.data(), emptyhits.data() + emptyhits.num_elements(), 0);

	plotLaserSquares(readings, curcoord, curangle, gridscale);
	plotEmptySquares(readings, curcoord, curangle, gridscale);
	writeGrid(grid);
}

void LaserPlot::plotLaserSquares(const LaserSensor::Readings &readings, const Coord &curcoord, float curangle, const CoordScale &gridscale) {
	for (int laser=0; laser<config.maxlasers; laser++) {
		const LaserSensor::DistAngleVec &distangles = readings[laser];

		for (LaserSensor::DistAngleVec::const_iterator i = distangles.begin(); i != distangles.end(); ++i) {
			if (abs(i->angle) > config.maxangle)
				continue;

			Coord coord = i->toCoord(curangle);

			Pos pos = gridscale.coordToPos(curcoord.x + coord.x, curcoord.y - coord.y);
			if (!(pos.x >= 0 && pos.x < laserhits.shape()[0] && pos.y >= 0 && pos.y < laserhits.shape()[1]))
				continue;

			laserhits[pos.x][pos.y][laser]++;
		}
	}
}

void LaserPlot::plotEmptySquares(const LaserSensor::Readings &readings, const Coord &curcoord, float curangle, const CoordScale &gridscale) {
	const LaserSensor::DistAngleVec &emptydistangle = readings[0];
	for (LaserSensor::DistAngleVec::const_iterator i = emptydistangle.begin(); i != emptydistangle.end(); ++i) {
		if (abs(i->angle) > config.maxangle)
			continue;

		for (float dist=.5; dist < i->dist; dist+=.5) {
			Coord coord = LaserSensor::DistAngle(dist, i->angle).toCoord(curangle);
			Pos pos = gridscale.coordToPos(curcoord.x + coord.x, curcoord.y - coord.y);
			if (!(pos.x >= 0 && pos.x < emptyhits.shape()[0] && pos.y >= 0 && pos.y < emptyhits.shape()[1]))
				break;

			emptyhits[pos.x][pos.y]++;
		}
	}
}

void LaserPlot::writeGrid(WorldGrid &grid) {
	for (int x=0; x<grid.getWidth(); x++) {
		for (int y=0; y<grid.getHeight(); y++) {
			bool possibleempty = true;
			for (int laser=0; laser<config.maxlasers; laser++) {
				if (laserhits[x][y][laser] >= config.minhits[laser]) {
					grid[Pos(x,y)] = config.squarelookup[laser];
					possibleempty = false;
					break;
				}
			}

			if (possibleempty && emptyhits[x][y] >= config.minemptyhits)
				grid[Pos(x,y)] = WorldGrid::EMPTY;
		}
	}
}

void LaserPlot::Config::readTree(const ptree &pt) {
	maxlasers = pt.get<int>("maxlasers");
	maxangle = pt.get<float>("maxangle_deg")/180*M_PI;

	emptyhitslaser = pt.get<int>("emptyhitslaser");
	minemptyhits = pt.get<int>("minemptyhits");

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

