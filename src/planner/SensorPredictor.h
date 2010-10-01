#ifndef SENSORPREDICTOR_H
#define SENSORPREDICTOR_H

#include "WorldGrid.h"
#include "types.h"

namespace pathsim {
	class SensorPredictor {
		public:
			virtual PosSet predictVision(const Pos &curpos, Dir curdir, const WorldGrid &grid) const =0;
	};
}

#endif

