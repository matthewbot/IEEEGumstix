#ifndef LASERPLOT_H
#define LASERPLOT_H

#include "ieee/drivers/laser/LaserSensor.h"
#include "ieee/shared/WorldGrid.h"
#include <boost/multi_array.hpp>

namespace ieee {
	class LaserPlot {
		public:
			struct Config {
				int maxlasers;
				float maxangle;

				const int *minhits;
				const WorldGrid::GridSquare *squarelookup;
			};

			LaserPlot(const Config &config, const LaserSensor::Readings &readings, const Coord &curcoord, float curangle, WorldGrid &grid, const CoordScale &gridscale);

			inline int getCount(int x, int y, int laser) const { return squares[x][y][laser]; }

		private:
			boost::multi_array<int, 3> squares;
	};
}

#endif
