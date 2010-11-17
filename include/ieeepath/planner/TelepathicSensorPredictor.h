#ifndef TELEPATHICSENSORPREDICTOR_H
#define TELEPATHICSENSORPREDICTOR_H

#include "ieeepath/planner/SensorPredictor.h"

namespace ieee {
	class TelepathicSensorPredictor : public SensorPredictor {
		public:
			TelepathicSensorPredictor(float range);
			
			virtual PosSet predictVision(const Coord &curpos, float curdir, const WorldGrid &grid) const;
			
		private:
			float range;
	};
}

#endif
