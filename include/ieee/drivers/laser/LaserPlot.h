#ifndef LASERPLOT_H
#define LASERPLOT_H

#include "ieee/drivers/laser/LaserSensor.h"
#include "ieee/shared/WorldGrid.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/multi_array.hpp>
#include <vector>

namespace ieee {
	class LaserPlot {
		public:
			struct Config {
				int maxlasers;
				float maxangle;

				int emptyhitslaser;
				int minemptyhits;

				std::vector<int> minhits;
				std::vector<WorldGrid::GridSquare> squarelookup;
				float extradist;

				void readTree(const boost::property_tree::ptree &pt);
				void writeTree(boost::property_tree::ptree &pt) const;
			};

			LaserPlot(const Config &config, const LaserSensor::Readings &readings, const Coord &curcoord, float curangle, WorldGrid &grid, const CoordScale &gridscale);

			inline int getLaserHits(int x, int y, int laser) const { return laserhits[x][y][laser]; }

		private:
			const Config &config;

			boost::multi_array<int, 3> laserhits;
			boost::multi_array<int, 2> emptyhits;

			void plotLaserSquares(const LaserSensor::Readings &readings, const Coord &curcoord, float curangle, const CoordScale &gridscale);
			void plotEmptySquares(const LaserSensor::Readings &readings, const Coord &curcoord, float curangle, const CoordScale &gridscale);
			void writeGrid(WorldGrid &grid);
	};
}

#endif
