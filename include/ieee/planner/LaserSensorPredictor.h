#ifndef LASERSENSORPREDICTOR_H
#define LASERSENSORPREDICTOR_H

#include "SensorPredictor.h"

namespace ieee {
	class LaserSensorPredictor : public SensorPredictor {
		public:
			LaserSensorPredictor(float maxdistance, float fieldofview, float fovstep);

			virtual PosSet predictVision(const Coord &curpos, float curdir, const WorldGrid &grid, const CoordScale &scale) const;

		private:
			float maxdistance;
			float fovstep;
			float fieldofview;
	};
}

#endif

