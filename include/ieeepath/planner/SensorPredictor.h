#ifndef SENSORPREDICTOR_H
#define SENSORPREDICTOR_H

#include "ieeepath/shared/WorldGrid.h"
#include "ieeepath/shared/types.h"

namespace ieeepath {
	class SensorPredictor {
		public:
			virtual PosSet predictVision(const Pos &curpos, Dir curdir, const WorldGrid &grid) const =0;
	};
}

#endif
