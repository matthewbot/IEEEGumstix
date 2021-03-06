#ifndef TELEPATHICSENSORPREDICTOR_H
#define TELEPATHICSENSORPREDICTOR_H

#include "ieee/planner/SensorPredictor.h"

namespace ieee {
	class TelepathicSensorPredictor : public SensorPredictor {
		public:
			TelepathicSensorPredictor(float range);
			
			virtual PosSet predictVision(const Coord &curpos, float curdir, const WorldGrid &grid, const CoordScale &scale) const;
			
		private:
			float range;
	};
}

#endif
