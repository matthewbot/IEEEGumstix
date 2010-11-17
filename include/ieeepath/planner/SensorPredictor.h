#ifndef SENSORPREDICTOR_H
#define SENSORPREDICTOR_H

#include "ieeepath/planner/NodeGrid.h"
#include "ieeepath/shared/types.h"

namespace ieee {
	class SensorPredictor {
		public:
			virtual PosSet predictVision(const Coord &curpos, float curdir, const WorldGrid &grid) const =0;
	};
}

#endif

