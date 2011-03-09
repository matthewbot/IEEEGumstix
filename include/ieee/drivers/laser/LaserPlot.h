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

				std::vector<int> minhits;
				std::vector<WorldGrid::GridSquare> squarelookup;

				void readTree(const boost::property_tree::ptree &pt);
			};

			LaserPlot(const Config &config, const LaserSensor::Readings &readings, const Coord &curcoord, float curangle, WorldGrid &grid, const CoordScale &gridscale);

			inline int getCount(int x, int y, int laser) const { return squares[x][y][laser]; }

		private:
			boost::multi_array<int, 3> squares;
	};
}

#endif
